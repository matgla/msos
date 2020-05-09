// This file is part of MSOS project. This is simple OS for embedded development devices.
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

#pragma once

#include <map>
#include <thread>
#include <vector>

#include <unistd.h>

#include "arch/armv7-m/registers.hpp"
#include "msos/kernel/process/process.hpp"

#include "msos/fs/i_file.hpp"

namespace msos
{
namespace arch
{
namespace x86
{

class CppProcess : public kernel::process::Process
{
public:
    CppProcess() = default;
    CppProcess(const CppProcess& process);
    CppProcess(const std::size_t process_entry, const std::size_t stack_size, std::size_t arg);
    ~CppProcess();

    const std::size_t* current_stack_pointer() const override;
    void current_stack_pointer(const std::size_t* stack_pointer) override;

    void block(void* semaphore) override;
    void unblock(void* semaphore) override;

    bool is_current_thread() const;

private:
    std::thread thread_;
};

} // namespace x86
} // namespace arch
} // namespace msos

