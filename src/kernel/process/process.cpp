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

#include <cstring>

#include "msos/kernel/process/process.hpp"
#include "msos/kernel/process/registers.hpp"

namespace msos
{
namespace kernel
{
namespace process
{

constexpr uint32_t default_psr_status = 0x01000000;

static pid_t pid_counter = 1;

Process::Process(const Process& parent, const std::size_t process_entry, const std::size_t return_address)
    : state_(State::Ready)
    , pid_(pid_counter++)
    , stack_size_(parent.stack_size())
    , stack_(new std::size_t[parent.stack_size()/(sizeof(std::size_t))]())
{
    uint8_t* stack_ptr = reinterpret_cast<uint8_t*>(stack_.get()) + stack_size_ - process_entry * sizeof(uint32_t) - sizeof(HardwareStoredRegisters) - sizeof(SoftwareStoredRegisters);
    std::size_t required_stack_size = process_entry * sizeof(std::size_t) + sizeof(HardwareStoredRegisters) + sizeof(SoftwareStoredRegisters);
    if (required_stack_size >= stack_size_)
    {
        printf ("Not enough space on stack in child task, required %d bytes.\n", required_stack_size);
    }
    
    std::memcpy(stack_.get(), parent.stack_.get(), parent.stack_size());
    HardwareStoredRegisters* hw_registers = reinterpret_cast<HardwareStoredRegisters*>(stack_ptr + sizeof(SoftwareStoredRegisters));
    hw_registers->r0 = 0;
    hw_registers->r1 = 1;
    hw_registers->r2 = 2;
    hw_registers->r3 = 3;

    SoftwareStoredRegisters* sw_registers = reinterpret_cast<SoftwareStoredRegisters*>(stack_ptr);

    sw_registers->r4 = 4;
    sw_registers->r5 = 5;
    sw_registers->r6 = 6;
    sw_registers->r7 = 7;
    sw_registers->r8 = 8;
    sw_registers->r9 = 9;
    sw_registers->r10 = 10;
    sw_registers->r11 = 11;
   
    hw_registers->psr = default_psr_status; 
    hw_registers->lr = 0;
    hw_registers->pc = return_address;
    sw_registers->lr = return_to_thread_mode_psp;
    current_stack_pointer_ = reinterpret_cast<std::size_t*>(stack_ptr);
}

Process::Process(const Process& process)
    : state_(process.state_)
    , pid_(process.pid_)
    , stack_size_(process.stack_size_)
{
    if (process.stack_ != nullptr)
    {
        stack_.reset(new std::size_t[process.stack_size()/sizeof(std::size_t)]);
        std::memcpy(stack_.get(), process.stack_.get(), stack_size_);
        current_stack_pointer_ = stack_.get() + stack_size_ - process.stack_usage();
    }
    else 
    {
        current_stack_pointer_ = process.current_stack_pointer_;
    }
}

Process::Process(const std::size_t process_entry, const std::size_t stack_size)
    : state_(State::Ready)
    , pid_(pid_counter++)
    , stack_size_(stack_size)
    , stack_(new std::size_t[stack_size/(sizeof(std::size_t))]())
{ 
    uint8_t* stack_ptr = reinterpret_cast<uint8_t*>(stack_.get()) + stack_size_ - sizeof(HardwareStoredRegisters) - sizeof(SoftwareStoredRegisters);
    std::size_t required_stack_size = sizeof(HardwareStoredRegisters) + sizeof(SoftwareStoredRegisters);
    if (required_stack_size >= stack_size_)
    {
        printf ("Not enough space on stack in child task, required %d bytes.\n", required_stack_size);
    }
    
    HardwareStoredRegisters* hw_registers = reinterpret_cast<HardwareStoredRegisters*>(stack_ptr + sizeof(SoftwareStoredRegisters));
    hw_registers->r0 = 0;
    hw_registers->r1 = 1;
    hw_registers->r2 = 2;
    hw_registers->r3 = 3;

    SoftwareStoredRegisters* sw_registers = reinterpret_cast<SoftwareStoredRegisters*>(stack_ptr);

    sw_registers->r4 = 4;
    sw_registers->r5 = 5;
    sw_registers->r6 = 6;
    sw_registers->r7 = 7;
    sw_registers->r8 = 8;
    sw_registers->r9 = 9;
    sw_registers->r10 = 10;
    sw_registers->r11 = 11;
   
    //uint32_t psp = get_psp();
    hw_registers->psr = default_psr_status; 
    hw_registers->lr = 0;
    hw_registers->pc = process_entry;
    sw_registers->lr = return_to_thread_mode_psp;
    current_stack_pointer_ = reinterpret_cast<std::size_t*>(stack_ptr);
}

Process::Process(std::size_t* stack_pointer, const std::size_t stack_size)
    : state_(State::Ready)
    , pid_(pid_counter++)
    , stack_size_(stack_size)
    , current_stack_pointer_(stack_pointer)
{
}

const std::size_t* Process::stack_pointer() const 
{
    return stack_.get();
}

std::size_t Process::stack_usage() const 
{
    return reinterpret_cast<const uint8_t*>(stack_.get()) + stack_size_ - reinterpret_cast<const uint8_t*>(current_stack_pointer_);
}

pid_t Process::pid() const 
{
    return pid_;
}

std::size_t Process::stack_size() const 
{
    return stack_size_;
}

const std::size_t* Process::current_stack_pointer()
{
    return current_stack_pointer_;
}

const std::size_t* Process::current_stack_pointer() const 
{
    return current_stack_pointer_;
}

void Process::current_stack_pointer(const std::size_t* stack_pointer)
{
    current_stack_pointer_ = stack_pointer;
}

} // namespace process     
} // namespace kernel     
} // namespace msos 

