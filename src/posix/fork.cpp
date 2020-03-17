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

#include "unistd.h"

#include <cstdint>
#include <cstdio>

#include "msos/kernel/process/context_switch.hpp"
#include "msos/kernel/process/process_manager.hpp"
#include "msos/kernel/process/scheduler.hpp"
#include "msos/kernel/process/process.hpp"
#include "msos/kernel/process/registers.hpp"
#include "msos/kernel/synchronization/mutex.hpp"

#include <hal/time/time.hpp>
#include <hal/core/criticalSection.hpp>

#include <stm32f10x.h>

extern "C"
{

    pid_t _fork(void);
    pid_t getpid();
    void dump_registers(void);

    pid_t process_fork(uint32_t sp, uint32_t return_address, msos::kernel::process::RegistersDump* registers);


    void SysTick_Handler(void);
    void PendSV_Handler(void);
}

static inline uint32_t get_PRIMASK()
{
    uint32_t mask;
    asm inline volatile(
        "mrs %0, primask\n" : "=r"(mask));
    return mask;
}

extern std::size_t __stack_start;
std::size_t* stack_start = &__stack_start;
constexpr std::size_t default_stack_size = 1024;

msos::kernel::process::ProcessManager* processes;

namespace msos
{
namespace kernel
{
namespace process
{

Scheduler* scheduler;
}
}
}


static bool was_initialized = false;
static bool first = true;
void __attribute__((naked)) PendSV_Handler(void)
{
    if (!first && !msos::kernel::process::scheduler->current_process_was_deleted())
    {

        asm volatile inline (
            "mrs r0, psp \n"
            "stmdb r0!, {r4 - r11, lr}\n"
        );

        std::size_t* stack;
        asm volatile inline (
            "mov %0, r0" : "=r" (stack)
        );
        msos::kernel::process::scheduler->current_process().current_stack_pointer(stack);
    }
    else
    {
        first = false;
    }

    asm volatile inline (
            "mov r0, %0\n" : : "r" (msos::kernel::process::scheduler->schedule_next())
    );

    asm volatile inline (
            "ldmia r0!, {r4 - r11, lr}\n"
            "msr psp, r0\n"
            );
    asm volatile inline (
        "bx lr\n"
    );
}


pid_t getpid()
{
    return msos::kernel::process::scheduler->current_process().pid();
}

pid_t root_process(const std::size_t process)
{
    if (processes != nullptr)
    {
        delete processes;
    }
    if (msos::kernel::process::scheduler != nullptr)
    {
        delete msos::kernel::process::scheduler;
    }

    processes = new msos::kernel::process::ProcessManager();
    msos::kernel::process::scheduler = new msos::kernel::process::Scheduler(*processes);

    auto& root_process = processes->create_process(process, default_stack_size);
    processes->print();
    msos::kernel::process::scheduler->schedule_next();
    NVIC_SetPriority(PendSV_IRQn, 0xff); /* Lowest possible priority */
    NVIC_SetPriority(SVCall_IRQn, 0x01); /* Lowest possible priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00); /* Highest possible priority */
    hal::time::Time::add_handler([](std::chrono::milliseconds time)
    {
        static std::chrono::milliseconds last_time = time;

        if (std::chrono::duration_cast<std::chrono::milliseconds>(time - last_time) >= std::chrono::milliseconds(100))
        {
            if (get_PRIMASK() != 1)
            {
                SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
            }
            last_time = time;
        }

    });
   return root_process.pid();
}


pid_t process_fork(uint32_t sp, uint32_t return_address, msos::kernel::process::RegistersDump* registers)
{
    auto& parent_process = msos::kernel::process::scheduler->current_process();
    /* sizeof(size_t), LR is pushed to stack in _fork function */
    std::size_t diff = (reinterpret_cast<std::size_t>(parent_process.stack_pointer()) + parent_process.stack_size()) - sp - sizeof(size_t);
    auto& child_process = processes->create_process(parent_process, diff, return_address, registers);

    processes->print();
    return child_process.pid();
}


extern "C"
{
pid_t _fork_p(uint32_t sp, uint32_t link_register);
}

static msos::kernel::process::RegistersDump registers;
static volatile uint32_t syscall_return_code = 0;

pid_t __attribute__((naked)) _fork_p(uint32_t sp, uint32_t link_register)
{
    asm volatile inline("push {r2, r3, r4, r5, lr}");

    asm volatile inline("mov r5, %0" : : "r"(&registers));
    asm volatile inline("mov r4, %0" : : "r"(sp));
    asm volatile inline("mov r1, %0" : : "r"(link_register));
    asm volatile inline("mov r2, %0" : : "r"(&syscall_return_code));
    asm volatile inline("mov r0, #3");
    asm volatile inline(//"isb   \n\t"
                        //"dsb   \n\t"
                        "svc 0 \n\t"
                        "wfi   \n\t"
                        "isb   \n\t"
                        "dsb   \n\t"
                        );
    asm volatile inline("mov r0, %0" : "=r"(syscall_return_code));
    asm volatile inline("pop {r2, r3, r4, r5, pc}");
}

// This function must ensure that stack is not touched inside, but may calls such functions
pid_t __attribute__((naked)) _fork(void)
{
    asm volatile inline ("mov r0, #7");
    asm volatile inline ("svc 0");

    asm volatile inline ("mov r0, %0" : : "r"(reinterpret_cast<uint32_t*>(&registers)));
    asm volatile inline ("stmia r0, {r1 - r12, lr}");
    asm volatile inline("isb\n\t"
                        "push {lr}\n\t");

    asm volatile inline(
                        "mov r1, lr\n\t"
                        "mrs r0, PSP\n\t"
                        "isb\n\t"
                        "bl _fork_p\n\t"
                        "pop {pc}\n\t"
    );

    asm volatile inline ("mov r0, #8");
    asm volatile inline ("svc 0");


    // return _fork_p(get_sp());
}

