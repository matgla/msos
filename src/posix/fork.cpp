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
    pid_t getpid();
    void dump_registers(void);

    pid_t process_fork(uint32_t sp, uint32_t return_address, msos::kernel::process::RegistersDump* registers);

    int was_current_process_deleted();
    void update_stack_pointer(const std::size_t* stack);
    void set_first_task_processed();

    const std::size_t* get_next_task();


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

int was_current_process_deleted()
{
    return msos::kernel::process::scheduler->current_process_was_deleted();
}

void update_stack_pointer(const std::size_t* stack)
{
    msos::kernel::process::scheduler->current_process().current_stack_pointer(stack);
}

void set_first_task_processed()
{
    first = false;
}

void __attribute__((naked)) PendSV_Handler(void)
{
    asm volatile inline (
        "mrs r0, psp \n"
        "stmdb r0!, {r4 - r11, lr}\n"
        "bl update_stack_pointer\n"
        "bl get_next_task\n"
        "ldmia r0!, {r4 - r11, lr}\n"
        "msr psp, r0\n"
        "bx lr\n"
    );
}

const std::size_t* get_next_task()
{
    return msos::kernel::process::scheduler->schedule_next();
}

pid_t getpid()
{
    return msos::kernel::process::scheduler->current_process().pid();
}

void pend()
{
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

extern "C"
{
    volatile int is_pendsv_blocked = false;
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
                if (!is_pendsv_blocked)
                {
                    pend();
                }
            }
            last_time = time;
        }

    });

    asm volatile inline(
        "mov r0, #9\n"
        "svc 0\n");

    while(true)
    {
    }
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
volatile msos::kernel::process::RegistersDump registers;
volatile uint32_t syscall_return_code = 0;

}

