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

#include <eul/container/static_vector.hpp>

#include "msos/kernel/process/fwd.hpp"
#include "msos/kernel/process/process.hpp"

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
    using ContainerType = eul::container::static_vector<Process, 10>;
    Process& create_process(std::size_t* stack_pointer, const std::size_t stack_size);
    Process& create_process(const Process& parent, const std::size_t process_entry, const std::size_t return_address);
    const ContainerType& get_processes() const;
    ContainerType& get_processes();
private:
    ContainerType processes_;
};

} // namespace process
} // namespace kernel 
} // namespace msos

