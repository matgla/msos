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
#include <string.h>
#include <cstdint>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

extern "C"
{
    enum SyscallNumber : int
    {
        SYSCALL_EXIT = 1,
        SYSCALL_KILL = 2,
    };

    int _gettimeofday(struct timeval* tv, void* tzvp);
    void _init();
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

    void trigger_syscall(SyscallNumber number, void* args, void* output);
}

namespace msos
{
namespace syscalls
{

void process_exit(int code);

} // namespace syscalls
} // namespace mso

