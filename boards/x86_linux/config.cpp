// This file is part of MS Keychain Gamer project. This is tiny game console.
// Copyright (C) 2020 Mateusz Stadnik
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

#include <config.hpp>

#include <arch/x86/cpp_scheduler.hpp>
// #include <msos/kernel/process/scheduler.hpp>

#include <msos/syscalls/syscalls.hpp>
#include <msos/posix/fsync.hpp>
#include <msos/os/sys/ioctl.h>

extern "C"
{
    char __heap_start = 0;
    char __heap_end = 0;

    int __wrap_write(int file, const char* ptr, int len)
    {
        return _write(file, ptr, len);
    }

    int __wrap_open(const char* filename, int flags)
    {
        return _open(filename, flags);
    }

    int __wrap_read(int file, char* ptr, int len)
    {
        return _read(file, ptr, len);
    }

    int __wrap_kill(int pid, int sig)
    {
        return _kill(pid, sig);
    }

    int __wrap_getpid(int n)
    {
        return _getpid(n);
    }

    int __wrap_close(int file)
    {
        return _close(file);
    }

    int __wrap_ioctl(int fd, uint32_t cmd, void* arg)
    {
        return _ioctl(fd, cmd, arg);
    }

    int __wrap_fsync(int fd)
    {
        return _fsync(fd);
    }

}

namespace msos
{

namespace
{
    msos::arch::x86::CppScheduler scheduler;
}

void system_config()
{
    kernel::process::Scheduler::set(&scheduler);
}

}
