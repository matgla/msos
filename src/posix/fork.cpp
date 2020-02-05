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

#include <hal/time/time.hpp>

#include <stm32f10x.h>

extern "C"
{
    pid_t _fork();
    pid_t getpid();

    void SysTick_Handler(void);
    void PendSV_Handler(void);
}


extern std::size_t __stack_start;
std::size_t* stack_start = &__stack_start;
constexpr std::size_t default_stack_size = 1024;

msos::kernel::process::ProcessManager* processes;
// msos::kernel::process::Scheduler* scheduler;

static bool was_initialized = false;
static bool first = true;
void __attribute__((naked)) PendSV_Handler(void)
{
    if (!first)
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
            "isb\n"
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

    auto& root_process = processes->create_process(process, 2048);
    processes->print();
    msos::kernel::process::scheduler->schedule_next();
    NVIC_SetPriority(PendSV_IRQn, 0xff); /* Lowest possible priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00); /* Highest possible priority */
    hal::time::Time::add_handler([](std::chrono::milliseconds)
    {
        volatile static int i = 0;
        i++;
        if (i > 100)
        {
            SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
        i = 0;
        }
    });
   //SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
   return root_process.pid();
}

pid_t _fork()
{
    auto& parent_process = msos::kernel::process::scheduler->current_process();
    std::size_t diff = (reinterpret_cast<std::size_t>(parent_process.stack_pointer()) + parent_process.stack_size()) - get_sp();
    std::size_t return_address = reinterpret_cast<std::size_t>(__builtin_return_address(0));
    auto& child_process = processes->create_process(parent_process, diff, return_address);

    printf("Child proccess forked with PID: %d\n", child_process.pid());
    processes->print();
    return child_process.pid();
}

