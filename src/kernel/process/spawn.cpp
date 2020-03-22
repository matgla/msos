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
#include "msos/kernel/process/scheduler.hpp"

extern msos::kernel::process::ProcessManager* processes;

pid_t spawn(void (*start_routine) (void *), void *arg)
{
    printf("Creating process with function: %p\n", start_routine);
    auto& child = processes->create_process(
        reinterpret_cast<std::size_t>(start_routine), 1024, reinterpret_cast<std::uint32_t>(arg));
    printf("Process created with pid: %d\n", child.pid());
    processes->print();
    return child.pid();
}
