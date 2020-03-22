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

#include "msos/kernel/process/spawn.hpp"

#include "msos/kernel/process/process.hpp"
#include "msos/kernel/process/context_switch.hpp"
#include "msos/kernel/process/scheduler.hpp"

#include "msos/syscalls/syscalls.hpp"

#include <hal/interrupt/systick.hpp>

msos::kernel::process::ProcessManager processes;

constexpr std::size_t default_stack_size = 1024;

pid_t spawn(void (*start_routine) (void *), void *arg)
{
    auto& child = processes.create_process(
        reinterpret_cast<std::size_t>(start_routine), default_stack_size, reinterpret_cast<std::uint32_t>(arg));
    return child.pid();
}

pid_t spawn_root_process(void (*start_routine) (void *), void *arg)
{
    hal::interrupt::disable_systick();
    msos::kernel::process::Scheduler::get().set_process_manager(processes);
    processes.create_process(reinterpret_cast<std::size_t>(start_routine), default_stack_size);
    msos::kernel::process::Scheduler::get().schedule_next();

    msos::process::initialize_context_switching();
    trigger_syscall(SyscallNumber::SYSCALL_START_ROOT_PROCESS, NULL, NULL);
    while(1) {}
}
