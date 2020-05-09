// This file is part of MSOS project.
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

#include <cstdio>

#include <hal/interrupt/pendsv.hpp>
#include <hal/interrupt/svc.hpp>
#include <hal/interrupt/systick.hpp>

#include "msos/syscalls/syscalls.hpp"
#include "msos/kernel/process/scheduler.hpp"
#include "msos/usart_printer.hpp"

#include "arch/armv7-m/pendsv_handler.hpp"
#include "arch/armv7-m/systick_handler.hpp"
#include "arch/armv7-m/svc_handler.hpp"


namespace msos
{
namespace process
{

void initialize_context_switching()
{
    hal::interrupt::set_systick_priority(0x00);
    hal::interrupt::set_svc_priority(0xfe);
    hal::interrupt::set_pendsv_priority(0xff);

    initialize_pendsv();
    initialize_systick();
    initialize_svc();

    hal::interrupt::set_systick_period(std::chrono::milliseconds(1));
}

} // namespace process
} // namespace msos

static UsartWriter writer;

extern "C"
{

void yield()
{
    trigger_syscall(SyscallNumber::YIELD, NULL, NULL);
}

const std::size_t* get_next_task()
{
    auto* scheduler = msos::kernel::process::Scheduler::get();
    if (!scheduler)
    {
        return nullptr;
    }
    scheduler->schedule_next();
    return scheduler->current_process()->current_stack_pointer();
}

void update_stack_pointer(const std::size_t* stack)
{
    auto* scheduler = msos::kernel::process::Scheduler::get();
    if (!scheduler)
    {
        // TODO: handle this crash -> panic()
        return;
    }
    auto* process = scheduler->current_process();
    if (!process)
    {
        // TODO: handle this crash -> panic()
        return;
    }
    process->current_stack_pointer(stack);
}

}
