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
#include <board.hpp>
#include <gsl/span>

#include <stm32f10x.h>
#include <core_cm3.h>

#include <sys/times.h>
#include <time.h>

#include <eul/filesystem/path.hpp>
#include <eul/utils/unused.hpp>

#include <hal/time/time.hpp>
#include <hal/time/sleep.hpp>

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
    clock_t _times (struct tms *buf);
    // int nanosleep(const struct timespec* req, struct timespec* rem);
}

static UsartWriter writer;

extern "C"
{
pid_t process_fork(uint32_t sp, uint32_t return_address);

}

int _gettimeofday(struct timeval* tv, void* tzvp)
{
    UNUSED1(tzvp);
    time_t t = 0;
    t = hal::time::Time::microseconds().count();
    tv->tv_sec = t / 1000000;
    tv->tv_usec = t - (tv->tv_sec * 1000000);
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
    const auto& scheduler = msos::kernel::process::Scheduler::get();
    if (scheduler.empty())
    {
        if (board::interfaces::usarts[0])
        {
            board::interfaces::usarts[0]->write(std::string_view(ptr, static_cast<std::size_t>(len)));
        }
        return len;
    }

    msos::fs::IFile* file = scheduler.current_process().get_file(fd);
    if (file)
    {
        return static_cast<int>(file->write(std::string_view(ptr, static_cast<std::size_t>(len))));
    }

    return 0;
}

int _read(int fd, char* ptr, int len)
{
    msos::fs::IFile* file = msos::kernel::process::Scheduler::get().current_process().get_file(fd);

    if (file)
    {
        return static_cast<int>(file->read(gsl::span<char>(ptr, static_cast<std::size_t>(len))));
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
    writer << "opening file: " <<  filename << endl;

    auto& vfs = msos::fs::Vfs::instance();
    eul::filesystem::path path(filename);

    std::unique_ptr<msos::fs::IFile> file;
    if ((flags & O_ACCMODE) == O_RDONLY)
    {
        file = vfs.get(path.lexically_normal().c_str());
        if (!file)
        {
            return -1;
        }
        int fd = msos::kernel::process::Scheduler::get().current_process().add_file(std::move(file));
        return fd;
    }
    else if ((flags & O_CREAT) == O_CREAT)
    {
        file = vfs.create(path.lexically_normal().c_str());
        if (!file)
        {
            return -1;
        }

        int fd = msos::kernel::process::Scheduler::get().current_process().add_file(std::move(file));
        return fd;
    }

    return 3;
}

pid_t _fork()
{
    return 0;
}

extern "C"
{
int nanosleep(const struct timespec* req, struct timespec* rem)
{
    UNUSED1(rem);
    hal::time::sleep(std::chrono::microseconds(req->tv_sec * 1000000 + req->tv_nsec / 1000));
    return 0;
}
}

clock_t _times (struct tms *buf)
{
    clock_t clk = hal::time::Time::microseconds().count();
    buf->tms_utime = 0;
    buf->tms_stime = clk;
    buf->tms_cutime = 0;
    buf->tms_cstime = 0;
    return clk;
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
