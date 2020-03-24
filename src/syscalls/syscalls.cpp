// This file is part of MSOS project. This is simple OS for embedded development devices.
// Copyright (C) 2019 Mateusz Stadnik
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <errno.h>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <gsl/span>

#include <hal/memory/heap.hpp>
#include <hal/core/criticalSection.hpp>
#include <board.hpp>

#include "msos/syscalls/syscalls.hpp"
#include "msos/fs/mount_points.hpp"
#include "msos/kernel/process/scheduler.hpp"
#include "msos/kernel/process/context_switch.hpp"

#include "msos/usart_printer.hpp"

static hal::UsartWriter writer;

extern "C"
{
    int _gettimeofday(struct timeval* tv, void* tzvp);
    void _exit(int code);
    int _kill(int pid, int sig);
    int _getpid(int n);
    caddr_t _sbrk(int incr);
    int _write(int file, const char* ptr, int len);
    int _read(int file, char* ptr, int len);
    int _isatty(int file);
    int _lseek(int file, int ptr, int dir);
    int _close(int file);
    int _fstat(int file, struct stat* st);
    int _open(const char* filename, int flags);
    pid_t _fork();

}

extern "C"
{
pid_t process_fork(uint32_t sp, uint32_t return_address);

}

int _gettimeofday(struct timeval* tv, void* tzvp)
{
    return 0;
}

void _exit(int code)
{
    trigger_syscall(SyscallNumber::SYSCALL_EXIT, &code, NULL);

    /* Life of this process finished, waiting for kill by scheduler */
    while(1);
}

int _kill(int pid, int sig)
{
    return 0;
}
int _getpid(int n)
{
    return 0;
}

extern char __heap_start;
extern char __heap_end;

static char* current_heap_end = &__heap_start;

char stdin_data[100];
int index = 0;
volatile bool new_line_readed = false;

void write_to_stdin(char c)
{
    if (index >= 254)
    {
        index = 0;
    }
    if (c == '\n' || c == '\r')
    {
        new_line_readed = true;
        c = '\n';
        stdin_data[index + 1] = 0;
    }
    stdin_data[index] = c;
    write(1, stdin_data + index, 1);
    ++index;
}

caddr_t _sbrk(int incr)
{
    if (current_heap_end + incr > (&__heap_end))
    {
        errno = ENOMEM;
        printf("Heap overflow!\n");
        return NULL;
    }

    char* previous_heap_end = current_heap_end;
    current_heap_end += incr;
    return static_cast<caddr_t>(previous_heap_end);
}

int _write(int fd, const char* ptr, int len)
{
    // TODO: temporary hack, this also should, write to process descriptor
    if (fd == 0 || fd == 1 || fd == 2)
    {
        board::interfaces::Usart1 usart;
        usart.write(std::string_view(ptr, len));
        return 0;
    }

    msos::fs::IFile* file = msos::kernel::process::Scheduler::get().current_process().get_file(fd);
    if (file)
    {
        return file->write(gsl::make_span<const uint8_t>(reinterpret_cast<const uint8_t*>(ptr), len));
    }

    return 0;
}

int _read(int fd, char* ptr, int len)
{
    if (fd == 0)
    {
        while (!new_line_readed) {}
        std::memcpy(ptr, stdin_data, index + 1);
        auto ind = index;
        new_line_readed = false;
        index = 0;
        return ind ;
    }

    msos::fs::IFile* file = msos::kernel::process::Scheduler::get().current_process().get_file(fd);

    if (file)
    {
        return file->read(gsl::make_span<uint8_t>(reinterpret_cast<uint8_t*>(ptr), len));
    }

    return 0;
}

int _isatty(int file)
{
    // if (file == 0)
    // {
    //     return 1;
    // }

    return 0;
}

int _lseek(int file, int ptr, int dir)
{
    // printf("lseek file: %d, ptr %d, dir %d\n", file, ptr, dir);

    return 0;
}

int _close(int file)
{
    return msos::kernel::process::Scheduler::get().current_process().remove_file(file);
}

int _fstat(int file, struct stat* st)
{
    // if (file == 0)
    // {
    //     st->st_mode = S_IFCHR;
    //     st->st_blksize = 255;
    //     st->st_size = index;
    // }
    return 0;
}

int _open(const char* filename, int flags)
{
    // printf("opening file: %s\n", filename);

    auto& mount_points = msos::fs::mount_points.get_mounted_points();

    msos::fs::IFileSystem* fs;
    std::string_view best_mount_point;
    std::string_view path(filename);

    for (auto& point : mount_points)
    {
        std::size_t index = path.find(point.point);
        if (index != std::string_view::npos)
        {
            if (point.point.size() > best_mount_point.size())
            {
                best_mount_point = point.point;
            }
        }
    }

    if (best_mount_point != "/")
    {
        path.remove_prefix(best_mount_point.size());
    }


    fs = msos::fs::mount_points.get_mounted_filesystem(best_mount_point);

    if (fs == nullptr)
    {
        errno = ENOENT;
        return -1;
    }

    std::unique_ptr<msos::fs::IFile> file;
    if ((flags & O_ACCMODE) == O_RDONLY)
    {
        file = fs->get(path);
        int fd = msos::kernel::process::Scheduler::get().current_process().add_file(std::move(file));
        return fd;
    }
    else if ((flags & O_CREAT) == O_CREAT)
    {
        file = fs->create(path);

        int fd = msos::kernel::process::Scheduler::get().current_process().add_file(std::move(file));
        return fd;
    }

    return 3;
}

pid_t _fork()
{
    return 0;
}


namespace msos
{
namespace syscalls
{

void process_exit(int code)
{
    msos::kernel::process::Scheduler::get().delete_process(msos::kernel::process::Scheduler::get().current_process().pid());
    msos::kernel::process::Scheduler::get().current_process_was_deleted(true);
    switch_to_next_task();
}

} // namespace syscalls
} // namespace msos
