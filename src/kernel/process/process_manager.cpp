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

#include "msos/kernel/process/process_manager.hpp"

#include <algorithm>
#include <cstdio>

#include <hal/core/criticalSection.hpp>

#include "msos/kernel/process/process.hpp"
#include <stm32f10x.h>


namespace msos
{
namespace kernel
{
namespace process
{

ProcessManager::ProcessManager() : processes_{}
{
}

Process& ProcessManager::create_process(std::size_t process_entry, std::size_t stack_size, uint32_t arg)
{
    processes_.emplace_back(process_entry, stack_size, arg);
    return processes_.back();
}

const ProcessManager::ContainerType& ProcessManager::get_processes() const
{
    return processes_;
}

ProcessManager::ContainerType& ProcessManager::get_processes()
{
    return processes_;
}

void ProcessManager::print() const
{
    printf("Process list\n");
    printf("============\n");
    for (const auto& process : processes_)
    {
        printf ("PID %d: stack = %p, current_stack = %p, stack_size = %d\n", process.pid(), process.stack_pointer(), process.current_stack_pointer(), process.stack_size());
    }
}

void ProcessManager::delete_process(pid_t pid)
{
    printf("Deleting process with pid %d\n", pid);
    auto it = std::find_if(processes_.begin(), processes_.end(), [pid](const Process& process) {
        return process.pid() == pid;
    });

    if (it != processes_.end())
    {
        processes_.erase(it);
    }
    print();
}

} // namespace process
} // namespace kernel
} // namespace msos

