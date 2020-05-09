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

#include "msos/kernel/process/fwd.hpp"

#include "msos/kernel/process/scheduler.hpp"

namespace msos
{
namespace kernel
{
namespace process
{

class CppThreadScheduler : public IScheduler
{
public:
    void delete_process(pid_t pid) override;

    const Process* current_process() const override;
    Process* current_process() override;
    void schedule_next() override;
    void unblock_all(void* semaphore) override;
};



} // namespace process
} // namespace kernel
} // namespace msos

