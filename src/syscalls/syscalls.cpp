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
#include <cstdio>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include <hal/memory/heap.hpp>
#include <board.hpp>

#include "msos/kernel/process/scheduler.hpp"

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
}

int _gettimeofday(struct timeval* tv, void* tzvp)
{
    return 0;
}

void _exit(int code)
{
    printf("Process exited\n");
    msos::kernel::process::scheduler->get_processes().delete_process(msos::kernel::process::scheduler->current_process().pid());
    msos::kernel::process::scheduler->current_process_was_deleted(true);
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

namespace hal
{
namespace memory
{

std::size_t get_heap_size()
{
    return (&__heap_end) - (&__heap_start);
}

std::size_t get_heap_usage()
{
    return current_heap_end - (&__heap_start);
}

} // namespace memory
} // namespace hal

int _write(int file, const char* ptr, int len)
{
    // TODO: temporary hack, this shoul really write to system file
    board::interfaces::Usart1 usart;
    usart.write(std::string_view(ptr, len));
    return 0;
}

int _read(int file, char* ptr, int len)
{
    return 0;
}

int _isatty(int file)
{
    return 0;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

int _close(int file)
{
    return 0;
}

int _fstat(int file, struct stat* st)
{
    return 0;
}
