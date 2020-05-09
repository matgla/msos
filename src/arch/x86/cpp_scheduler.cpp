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

#include "arch/x86/cpp_scheduler.hpp"

namespace msos
{
namespace arch
{
namespace x86
{

using Manager = kernel::process::ProcessManager<CppProcess>;


CppScheduler::CppScheduler()
{

}

void CppScheduler::delete_process(pid_t pid)
{

}

const kernel::process::Process* CppScheduler::current_process() const
{
    for (const auto& process : Manager::get().get_processes())
    {
        if (process.is_current_thread())
        {
            return &process;
        }
    }
}

kernel::process::Process* CppScheduler::current_process()
{
    for (auto& process : Manager::get().get_processes())
    {
        if (process.is_current_thread())
        {
            return &process;
        }
    }
}

void CppScheduler::schedule_next()
{

}

void CppScheduler::unblock_all(void* semaphore)
{

}

} // namespace x86
} // namespace arch
} // namespace msos

