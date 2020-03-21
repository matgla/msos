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
#include <fcntl.h>
#include <gsl/span>

#include <hal/memory/heap.hpp>
#include <hal/core/criticalSection.hpp>
#include <board.hpp>

#include "msos/fs/mount_points.hpp"
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
    int _open(const char* filename, int flags);

}

extern "C"
{
pid_t process_fork(uint32_t sp, uint32_t return_address);

}

volatile uint32_t old;
volatile uint32_t counter = 0;

static __inline__ uint32_t get_psp()
{
    uint32_t sp;
    asm ("mrs %0, psp" : "=r"(sp));
    return sp;
}

void __attribute__((naked)) SVC_Handler()
{
    uint32_t number;
    asm volatile("mov %0, r0" :"=r"(number));
    if (number == 1)
    {
        old = NVIC->ISER[0];
        NVIC->ICER[0] = 0xffffffff;
        __disable_irq();
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
        --counter;
    }
    else if (number == 3)
    {
        /* this should be pure assembly */
        uint32_t sp;

        asm volatile inline(
            "push {r0, lr}\n\t"
            "mov r0, r4\n\t"
            "push {r2}\n\t"
            "mov r2, r5\n\t"
            "bl process_fork\n\t"
            "pop {r2}\n\t"
            "str r0, [r2, #0]\n\t"
            "pop {r0, pc}\n\t"
        );

        // uint32_t return_address;
        // uint32_t* val;

        // asm volatile inline("mov %0, r2" : "=r"(val));
        // asm volatile inline("mov %0, r1" : "=r"(return_address));

        // uint32_t pid = process_fork(sp, return_address);
        // *val = pid;
        // asm volatile inline("STR %0, [r2]" : : "r"(pid));
    }
    else if (number == 7)
    {
        NVIC_DisableIRQ(PendSV_IRQn);
    }
    else if (number == 8)
    {
        NVIC_EnableIRQ(PendSV_IRQn);
    }
    else if (number == 9)
    {
        asm volatile inline(
            "bl get_next_task\n"
            "ldmia r0!, {r4 - r11, lr}\n"
            "msr psp, r0\n"
            "bx lr\n");
    }
    else if (number == 10)
    {
        msos::kernel::process::scheduler->delete_process(msos::kernel::process::scheduler->current_process().pid());
        msos::kernel::process::scheduler->current_process_was_deleted(true);
        asm volatile inline(
            "bl get_next_task\n"
            "ldmia r0!, {r4 - r11, lr}\n"
            "msr psp, r0\n"
            "bx lr\n");
    }
}


int _gettimeofday(struct timeval* tv, void* tzvp)
{
    return 0;
}

void _exit(int code)
{
    printf("Process exited with code: %d\n", code);
    asm volatile inline(
        "mov r0, #10\n\t"
        "svc 0\n\t"
        "isb\n\t"
        "dsb\n\t"
        "wfi\n\t"
    );
    // hal::core::startCriticalSection();
    // disable pend sv
    // hal::core::stopCriticalSection();
    // enable pend sv
    // SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

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

int _write(int fd, const char* ptr, int len)
{
    // TODO: temporary hack, this also should, write to process descriptor
    if (fd == 0 || fd == 1 || fd == 2)
    {
        board::interfaces::Usart1 usart;
        usart.write(std::string_view(ptr, len));
        return 0;
    }

    msos::fs::IFile* file = msos::kernel::process::scheduler->current_process().get_file(fd);
    if (file)
    {
        return file->write(gsl::make_span<const uint8_t>(reinterpret_cast<const uint8_t*>(ptr), len));
    }

    return 0;
}

int _read(int fd, char* ptr, int len)
{
    msos::fs::IFile* file = msos::kernel::process::scheduler->current_process().get_file(fd);

    if (file)
    {
        return file->read(gsl::make_span<uint8_t>(reinterpret_cast<uint8_t*>(ptr), len));
    }

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
    return msos::kernel::process::scheduler->current_process().remove_file(file);
}

int _fstat(int file, struct stat* st)
{
    return 0;
}

int _open(const char* filename, int flags)
{
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
    printf("Flags: %d\n", flags);
    if ((flags & O_ACCMODE) == O_RDONLY)
    {
        file = fs->get(path);
        int fd = msos::kernel::process::scheduler->current_process().add_file(std::move(file));
        printf("Got file: %s, fd: %d\n", filename, fd);
        return fd;
    }
    else if ((flags & O_CREAT) == O_CREAT)
    {
        file = fs->create(path);

        int fd = msos::kernel::process::scheduler->current_process().add_file(std::move(file));
        printf("File created %s, fd: %d\n", filename, fd);

        return fd;
    }

    return 3;
}
