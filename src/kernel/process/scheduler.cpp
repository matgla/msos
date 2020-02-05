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

const std::size_t* Scheduler::schedule_next()
{
    if (processes_.get_processes().empty())
    {
        return nullptr;
    }

    auto next = get_next();
    if (next->get_state() == Process::State::Running)
    {
        return current_process_->current_stack_pointer();
    }

    if (next->get_state() == Process::State::Ready)
    {
        if (current_process_->get_state() == Process::State::Blocked)
        printf("Current is: %d\n", current_process_->pid());
        printf("Ready is: %d\n", next->pid());
        current_process_ = next;
        return current_process_->current_stack_pointer();
    }

    return nullptr;
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

