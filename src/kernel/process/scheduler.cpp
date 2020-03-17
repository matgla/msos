// This file is part of MSOS project.
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

#include "msos/kernel/process/scheduler.hpp"

#include "msos/kernel/process/process_manager.hpp"

#include <cstdio>
#include <stm32f10x.h>

namespace msos
{
namespace kernel
{
namespace process
{

// static Process* current_process;
Scheduler::Scheduler(ProcessManager& processes)
    : processes_(processes)
    , current_process_(processes_.get_processes().begin())
{
}

const Process& Scheduler::current_process() const
{
    return *current_process_;
}

Process& Scheduler::current_process()
{
    return *current_process_;
}

ProcessManager::ContainerType::iterator Scheduler::get_next()
{
    return std::next(current_process_) == processes_.get_processes().end() ?
        processes_.get_processes().begin() : std::next(current_process_);
}

void Scheduler::delete_process(pid_t pid)
{
    if (pid == current_process_->pid())
    {
        current_process_ = get_next();
    }
    processes_.delete_process(pid);
}

const std::size_t* Scheduler::schedule_next()
{
    current_process_was_deleted_ = false;
    if (processes_.get_processes().empty())
    {
        return nullptr;
    }

    auto next = get_next();
    while (next->get_state() != Process::State::Running)
    {
        current_process_ = next;
        if (current_process_->get_state() == Process::State::Ready || current_process_->get_state() == Process::State::Running)
        {
            return current_process_->current_stack_pointer();
        }
        next = get_next();
    }

    return current_process_->current_stack_pointer();
}

void Scheduler::unblock_all()
{
    for (auto& process : processes_.get_processes())
    {
        process.unblock();
    }
}

} // namespace process
} // namespace kernel
} // namespace msos

