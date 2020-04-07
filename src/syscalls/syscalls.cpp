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

#include <eul/filesystem/path.hpp>
#include <eul/utils/unused.hpp>

#include "msos/syscalls/syscalls.hpp"
#include "msos/fs/vfs.hpp"
#include "msos/kernel/process/scheduler.hpp"
#include "msos/kernel/process/context_switch.hpp"

#include "msos/usart_printer.hpp"

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

static UsartWriter writer;

extern "C"
{
pid_t process_fork(uint32_t sp, uint32_t return_address);

}

int _gettimeofday(struct timeval* tv, void* tzvp)
{
    UNUSED2(tv, tzvp);
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
    UNUSED2(pid, sig);
    return 0;
}
int _getpid(int n)
{
    UNUSED1(n);
    return 0;
}

extern char __heap_start;
extern char __heap_end;

static char* current_heap_end = &__heap_start;

char stdin_data[100];
int writer_position = 0;
volatile bool new_line_readed = false;

void write_to_stdin(char c)
{
    if (writer_position >= 99)
    {
        writer_position = 0;
    }
    if (c == 127)
    {
        --writer_position;
        char backspace = '\b';
        char space = ' ';
        write(1, &backspace, 1);
        write(1, &space, 1);
        write(1, &backspace, 1);
        return;
    }
    if (c == '\n' || c == '\r')
    {
        new_line_readed = true;
        c = '\n';
        stdin_data[writer_position + 1] = 0;
    }
    stdin_data[writer_position] = c;
    write(1, stdin_data + writer_position, 1);
    ++writer_position;
}

caddr_t _sbrk(int incr)
{
    if (current_heap_end + incr > (&__heap_end))
    {
        errno = ENOMEM;
        // printf("Heap overflow!\n");
        return NULL;
    }

    char* previous_heap_end = current_heap_end;
    current_heap_end += incr;
    return static_cast<caddr_t>(previous_heap_end);
}

int _write(int fd, const char* ptr, int len)
{
    msos::fs::IFile* file = msos::kernel::process::Scheduler::get().current_process().get_file(fd);
    if (file)
    {
        return file->write(std::string_view(ptr, len));
    }

    return 0;
}

int _read(int fd, char* ptr, int len)
{
    if (fd == 0)
    {
        while (!new_line_readed) {}
        std::memcpy(ptr, stdin_data, writer_position + 1);
        auto ind = writer_position;
        new_line_readed = false;
        writer_position = 0;
        return ind ;
    }

    msos::fs::IFile* file = msos::kernel::process::Scheduler::get().current_process().get_file(fd);

    if (file)
    {
        return file->read(gsl::make_span<char>(ptr, len));
    }

    return 0;
}

int _isatty(int file)
{
    UNUSED1(file);
    // if (file == 0)
    // {
    //     return 1;
    // }

    return 0;
}

int _lseek(int file, int ptr, int dir)
{
    UNUSED3(file, ptr, dir);
    // printf("lseek file: %d, ptr %d, dir %d\n", file, ptr, dir);

    return 0;
}

int _close(int file)
{
    return msos::kernel::process::Scheduler::get().current_process().remove_file(file);
}

int _fstat(int file, struct stat* st)
{
    UNUSED2(file, st);
    // if (file == 0)
    // {
    //     st->st_mode = S_IFCHR;
    //     st->st_blksize = 255;
    //     st->st_size = writer_position;
    // }
    return 0;
}

int _open(const char* filename, int flags)
{
    // printf("opening file: %s\n", filename);

    auto& vfs = msos::fs::Vfs::instance();
    eul::filesystem::path path(filename);

    std::unique_ptr<msos::fs::IFile> file;
    if ((flags & O_ACCMODE) == O_RDONLY)
    {
        file = vfs.get(path.lexically_normal().c_str());
        int fd = msos::kernel::process::Scheduler::get().current_process().add_file(std::move(file));
        return fd;
    }
    else if ((flags & O_CREAT) == O_CREAT)
    {
        file = vfs.create(path.lexically_normal().c_str());

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
    UNUSED1(code);
    msos::kernel::process::Scheduler::get().delete_process(msos::kernel::process::Scheduler::get().current_process().pid());
    msos::kernel::process::Scheduler::get().current_process_was_deleted(true);
    switch_to_next_task();
}

} // namespace syscalls
} // namespace msos
