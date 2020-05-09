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

#include <cstdint>

#include <vector>
#include <array>
#include <memory>
#include <list>

#include <sys/types.h>

#include "arch/armv7-m/registers.hpp"
#include "msos/kernel/process/process.hpp"

#include "msos/fs/i_file.hpp"

extern "C"
{
    pid_t root_process(const std::size_t process);
}

namespace msos
{
namespace arch
{
namespace armv7m
{

/* TODO: This shouldn't be placed directly in kernel, but in some abstraction */

constexpr uint32_t return_to_handler_mode_msp = 0xfffffff1;
constexpr uint32_t return_to_thread_mode_msp  = 0xfffffff9;
constexpr uint32_t return_to_thread_mode_psp  = 0xfffffffd;

constexpr uint32_t return_to_handler_mode_with_fp_msp = 0xffffffe1;
constexpr uint32_t return_to_thread_mode_with_fp_msp  = 0xffffffe9;
constexpr uint32_t return_to_thread_mode_with_fp_psp  = 0xffffffed;


class ArmProcess : public kernel::process::Process
{
public:

    ArmProcess() = default;
    ArmProcess(const ArmProcess& process);
    ArmProcess(const std::size_t process_entry, const std::size_t stack_size, std::size_t arg);

    std::size_t stack_size() const;
    const std::size_t* current_stack_pointer();
    const std::size_t* current_stack_pointer() const override;
    const std::size_t* stack_pointer() const;
    void current_stack_pointer(const std::size_t* stack_pointer) override;
    std::size_t stack_usage() const;

    void block(void* semaphore) override;
    void unblock(void* semaphore) override;

private:
    bool validate_stack() const;
    uint32_t patch_register(const ArmProcess& parent, uint32_t reg);
    std::size_t stack_size_;
    std::unique_ptr<std::size_t[]> stack_;
    const std::size_t* current_stack_pointer_;
    std::vector<std::size_t> locks_;
};

} // namespace armv7m
} // namespace arch
} // namespace msos

