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

#pragma once

#include <list>

#include "msos/kernel/process/fwd.hpp"
#include "msos/kernel/process/process.hpp"
#include "msos/kernel/process/process_manager.hpp"

namespace msos
{
namespace kernel
{
namespace process
{

class Scheduler
{
public:
    static Scheduler& get();

    void set_process_manager(ProcessManager& manager);

    void delete_process(pid_t pid);

    const Process& current_process() const;
    Process& current_process();
    const std::size_t* schedule_next();
    void unblock_all(void* semaphore);
    ProcessManager& get_processes()
    {
        return *processes_;
    }

    bool current_process_was_deleted()
    {
        return current_process_was_deleted_;
    }

    void current_process_was_deleted(bool v)
    {
        current_process_was_deleted_ = v;
    }

private:
    Scheduler();

    ProcessManager::ContainerType::iterator get_next();

    ProcessManager* processes_;
    ProcessManager::ContainerType::iterator current_process_;
    bool current_process_was_deleted_ = false;
};

} // namespace process
} // namespace kernel
} // namespace msos

