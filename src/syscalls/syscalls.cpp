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
#include <hal/core/criticalSection.hpp>
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
    void SVC_Handler();

}
pid_t process_fork(uint32_t sp, uint32_t return_address);

volatile uint32_t old;
volatile uint32_t counter = 0;

static __inline__ uint32_t get_psp()
{
    uint32_t sp;
    asm ("mrs %0, psp" : "=r"(sp));
    return sp;
}

void SVC_Handler()
{
    uint32_t number;
    asm volatile("mov %0, r0" :"=r"(number));
    if (number == 1)
    {
        old = NVIC->ISER[0];
        NVIC->ICER[0] = 0xffffffff;
        __disable_irq();
        __disable_fault_irq();
        ++counter;
    }
    else if (number == 2 && counter != 1)
    {
        --counter;
    }
    else if (number == 2 && counter == 1)
    {
        NVIC->ISER[0] = old;
        __enable_irq();
        __enable_fault_irq();
        --counter;
    }
    else if (number == 3)
    {
        uint32_t return_address;
        uint32_t* val;

        asm volatile inline("mov %0, r1" : "=r"(return_address));
        asm volatile inline("mov %0, r2" : "=r"(val));
        printf("Forking process: %x, %p\n", return_address, val);

        uint32_t pid = process_fork(get_psp(), return_address);
        *val = pid;
        // asm volatile inline("STR %0, [r2]" : : "r"(pid));
    }
}


int _gettimeofday(struct timeval* tv, void* tzvp)
{
    return 0;
}

void _exit(int code)
{
    hal::core::startCriticalSection();
    printf("Process exited with code: %d\n", code);
    msos::kernel::process::scheduler->delete_process(msos::kernel::process::scheduler->current_process().pid());
    msos::kernel::process::scheduler->current_process_was_deleted(true);
    printf("Enable interrupts \n");
    hal::core::stopCriticalSection();
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

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
