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

#include "arch/armv7-m/normal_scheduler.hpp"

#include <cstdio>

#include "msos/kernel/process/process_manager.hpp"
#include "arch/armv7-m/arm_process.hpp"


namespace msos
{
namespace arch
{
namespace armv7m
{

using Manager = kernel::process::ProcessManager<ArmProcess>;

NormalScheduler::NormalScheduler()
    : current_process_(Manager::get().get_processes().begin())
{
}

const kernel::process::Process* NormalScheduler::current_process() const
{
    return current_process_ == Manager::get().get_processes().end() ? nullptr : &(*current_process_);
}

kernel::process::Process* NormalScheduler::current_process()
{
    return current_process_ == Manager::get().get_processes().end() ? nullptr : &(*current_process_);
}

Manager::ContainerType::iterator NormalScheduler::get_next()
{
    return std::next(current_process_) == Manager::get().get_processes().end() ?
        Manager::get().get_processes().begin() : std::next(current_process_);
}

void NormalScheduler::delete_process(pid_t pid)
{
    if (pid == current_process_->pid())
    {
        current_process_ = get_next();
    }
    Manager::get().delete_process(pid);
}

void NormalScheduler::schedule_next()
{
    if (Manager::get().get_processes().empty())
    {
        return;
    }

    auto next = get_next();
    while (next->get_state() != kernel::process::Process::State::Running)
    {
        current_process_ = next;
        if (current_process_->get_state() == kernel::process::Process::State::Ready || current_process_->get_state() == kernel::process::Process::State::Running)
        {
            return;
        }
        next = get_next();
    }

}

void NormalScheduler::unblock_all(void* semaphore)
{
    for (auto& process : Manager::get().get_processes())
    {
        process.unblock(semaphore);
    }
}

} // namespace process
} // namespace kernel
} // namespace msos

