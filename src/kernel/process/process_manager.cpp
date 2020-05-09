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

#include "msos/kernel/process/process.hpp"


namespace msos
{
namespace kernel
{
namespace process
{

// ProcessManager& ProcessManager::get()
// {
//     static ProcessManager p;
//     return p;
// }

// ProcessManager::ProcessManager() : processes_{}
// {
// }

// Process& ProcessManager::create_process(std::size_t process_entry, std::size_t stack_size, std::size_t arg)
// {
//     processes_.emplace_back(process_entry, stack_size, arg);
//     return processes_.back();
// }

// const ProcessManager::ContainerType& ProcessManager::get_processes() const
// {
//     return processes_;
// }

// ProcessManager::ContainerType& ProcessManager::get_processes()
// {
//     return processes_;
// }

// void ProcessManager::print() const
// {
// }

// void ProcessManager::delete_process(pid_t pid)
// {
//     auto it = std::find_if(processes_.begin(), processes_.end(), [pid](const Process& process) {
//         return process.pid() == pid;
//     });

//     if (it != processes_.end())
//     {
//         processes_.erase(it);
//     }
//     print();
// }

} // namespace process
} // namespace kernel
} // namespace msos

