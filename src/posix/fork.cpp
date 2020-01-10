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
msos::kernel::process::Scheduler* scheduler;

static bool was_initialized = false;
static bool first = true;
void __attribute__((naked)) PendSV_Handler(void)
{
    printf("Context switch\n");
    if (!first){
        scheduler->current_process().current_stack_pointer(get_psp() - 9);
    }
    else 
    {
        first = false;
    }
    std::size_t* next_sp = scheduler->schedule_next();
    printf("Next stack pointer %p\n", next_sp);
    context_switch(reinterpret_cast<std::size_t>(next_sp));
}


pid_t getpid()
{
    return scheduler->current_process().pid(); 
}

pid_t _fork()
{
    if (!processes)
    {
        processes = new msos::kernel::process::ProcessManager();
        scheduler = new msos::kernel::process::Scheduler(*processes);
    }
    printf("Creating first process\n");
    // TODO: move from there 
    if (!was_initialized)
    {
        printf("Stack start: %x\n", &__stack_start);
        processes->create_process(stack_start, 2048);
        scheduler->schedule_next();
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
        printf("Added handler\n");
        was_initialized = true;
    }
    
    const auto& parent_process = scheduler->current_process();
    std::size_t diff = get_sp() - reinterpret_cast<std::size_t>(&__stack_start);
    printf("Stack diff: %d\n", diff);
    std::size_t return_address = reinterpret_cast<std::size_t>(__builtin_return_address(0));
    printf("Created with pc: %x\n", return_address);
    auto& child_process = processes->create_process(parent_process, diff, return_address);
    
    printf("Child proccess forked with PID: %d\n", child_process.pid());
    return child_process.pid();
}

