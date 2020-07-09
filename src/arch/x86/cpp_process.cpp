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

#include "arch/x86/cpp_process.hpp"

#include <algorithm>

extern "C"
{
    void switch_to_next_task()
    {

    }
}

namespace msos
{
namespace arch
{
namespace x86
{

static pid_t pid_counter = 1;


CppProcess::CppProcess(const CppProcess& process)
    : Process(process)
{
}

CppProcess::~CppProcess()
{
    pthread_cancel(thread_.native_handle());
}

CppProcess::CppProcess(const std::size_t process_entry, const std::size_t stack_size, std::size_t arg)
    : Process(State::Ready, pid_counter++)
{
    thread_ = std::thread(reinterpret_cast<void(*)(void*)>(process_entry), nullptr);
}

const std::size_t* CppProcess::current_stack_pointer() const
{
}

void CppProcess::current_stack_pointer(const std::size_t* stack_pointer)
{
}

void CppProcess::block(void* semaphore)
{
}

void CppProcess::unblock(void* semaphore)
{
}

bool CppProcess::is_current_thread() const
{
    return std::this_thread::get_id() == thread_.get_id();
}

} // namespace x86
} // namespace arch
} // namespace msos

