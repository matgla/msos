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

#include "msos/kernel/process/scheduler.hpp"

#include "arch/x86/cpp_process.hpp"

namespace msos
{
namespace arch
{
namespace x86
{

class CppScheduler : public kernel::process::IScheduler
{
public:
    CppScheduler();
    void delete_process(pid_t pid) override;

    const kernel::process::Process* current_process() const override;
    kernel::process::Process* current_process() override;
    void schedule_next() override;
    void unblock_all(void* semaphore) override;
private:

    kernel::process::ProcessManager<CppProcess>::ContainerType::iterator get_next();

    kernel::process::ProcessManager<CppProcess>::ContainerType::iterator current_process_;
};

} // namespace x86
} // namespace arch
} // namespace msos

