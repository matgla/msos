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

#include <cstdio>

#include <hal/core/criticalSection.hpp>

#include "msos/kernel/process/process.hpp"


namespace msos
{
namespace kernel
{
namespace process
{

ProcessManager::ProcessManager() : processes_{} 
{
}

Process& ProcessManager::create_process(const Process& parent, const std::size_t process_entry, const std::size_t return_address)
{
    hal::core::startCriticalSection();
    processes_.emplace_back(parent, process_entry, return_address);
    hal::core::stopCriticalSection();
    return processes_.back();
}

Process& ProcessManager::create_process(std::size_t process_entry, std::size_t stack_size)
{
    hal::core::startCriticalSection();
    processes_.emplace_back(process_entry, stack_size);
    hal::core::stopCriticalSection();
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

} // namespace process     
} // namespace kernel 
} // namespace msos 

