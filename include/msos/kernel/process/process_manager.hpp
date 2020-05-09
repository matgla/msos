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

#pragma once

#include <algorithm>
#include <cstdlib>
#include <list>

#include <unistd.h>

#include "msos/kernel/process/process.hpp"

namespace msos
{
namespace kernel
{
namespace process
{

template <typename ProcessType>
class ProcessManager
{
public:
    using SelfType = ProcessManager<ProcessType>;
    using ContainerType = std::list<ProcessType>;
    static SelfType& get()
    {
        static SelfType t;
        return t;
    }

    Process& create_process(std::size_t process_entry, std::size_t stack_size, std::size_t arg = 0)
    {
        processes_.emplace_back(process_entry, stack_size, arg);
        return processes_.back();
    }

    void delete_process(pid_t pid)
    {
        auto it = std::find_if(processes_.begin(), processes_.end(), [pid](const Process& process) {
            return process.pid() == pid;
        });

        if (it != processes_.end())
        {
            processes_.erase(it);
        }
    }

    const ContainerType& get_processes() const
    {
        return processes_;
    }

    ContainerType& get_processes()
    {
        return processes_;
    }

private:

    ContainerType processes_;
};

} // namespace process
} // namespace kernel
} // namespace msos

