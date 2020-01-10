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

#include <memory>
#include <list>

#include <sys/types.h>

namespace msos
{
namespace kernel
{
namespace process 
{

/* TODO: This shouldn't be placed directly in kernel, but in some abstraction */

constexpr uint32_t return_to_handler_mode_msp = 0xfffffff1;
constexpr uint32_t return_to_thread_mode_msp  = 0xfffffff9;
constexpr uint32_t return_to_thread_mode_psp  = 0xfffffffd;

constexpr uint32_t return_to_handler_mode_with_fp_msp = 0xffffffe1;
constexpr uint32_t return_to_thread_mode_with_fp_msp  = 0xffffffe9;
constexpr uint32_t return_to_thread_mode_with_fp_psp  = 0xffffffed;


class Process 
{
public:
    Process() = default;
    Process(const Process& parent, const std::size_t process_entry, const std::size_t return_address);
    Process(std::size_t* stack_pointer, const std::size_t stack_size);

    pid_t pid() const;
    std::size_t stack_size() const;
    std::size_t* current_stack_pointer(); 
    const std::size_t* current_stack_pointer() const;
    void current_stack_pointer(std::size_t* stack_pointer);        
    void kill();
private: 
    enum class State : uint8_t 
    {
        Ready,
        Blocked, 
        Running,
        Killed
    };

    State state_;
    pid_t pid_;
    std::size_t stack_size_;
    std::unique_ptr<std::size_t[]> stack_;
    std::size_t* stack_pointer_;
};

} // namespace process
} // namespace kernel
} // namespace msos

