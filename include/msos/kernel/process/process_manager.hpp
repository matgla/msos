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

#include <vector>

#include "msos/kernel/process/fwd.hpp"
#include "msos/kernel/process/process.hpp"
#include "msos/kernel/process/registers.hpp"

namespace msos
{
namespace kernel
{
namespace process
{

class ProcessManager
{
public:
    ProcessManager();
    using ContainerType = std::list<Process>;
    Process& create_process(const Process& parent, const std::size_t process_entry, const std::size_t return_address, RegistersDump* registers);

    Process& create_process(std::size_t process_entry, std::size_t stack_size, uint32_t arg = 0);

    void delete_process(pid_t pid);

    const ContainerType& get_processes() const;
    ContainerType& get_processes();
    void print() const;
private:
    ContainerType processes_;
};

} // namespace process
} // namespace kernel
} // namespace msos

