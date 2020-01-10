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
    , stack_pointer_(stack_.get())
{
    printf("Stack pointer in child: %p\n", stack_pointer_);
    printf("Process entry: %x\n", process_entry);
    uint8_t* stack_ptr = reinterpret_cast<uint8_t*>(stack_.get()) + process_entry;

    HardwareStoredRegisters* hw_registers = reinterpret_cast<HardwareStoredRegisters*>(stack_ptr + sizeof(SoftwareStoredRegisters));
    hw_registers->r0 = 0;
    hw_registers->r1 = 0xe;
    hw_registers->r2 = 0xa;
    hw_registers->r3 = 0xd;

    SoftwareStoredRegisters* sw_registers = reinterpret_cast<SoftwareStoredRegisters*>(stack_ptr);

    std::memcpy(stack_.get(), parent.stack_.get(), parent.stack_size());
    sw_registers->r4 = 0xf;
    sw_registers->r5 = 0xa;
    sw_registers->r6 = 0xc;
    sw_registers->r7 = 0xe;
    sw_registers->r8 = 0x8;
    sw_registers->r9 = 0x9;
    sw_registers->r10 = 0x10;
    sw_registers->r11 = 0x11;
   //uint32_t psp = get_psp();
    //printf ("Current parent psp: %x, parent stack start: %p\n", psp, parent_.);
    hw_registers->psr = default_psr_status;
    hw_registers->lr = 0;
    printf("Return address: %x\n", return_address);
    hw_registers->pc = return_address;
    //hw_registers->lr = return_address;
    sw_registers->lr = return_to_handler_mode_msp;
    stack_pointer_ = reinterpret_cast<std::size_t*>(stack_ptr);
}

Process::Process(std::size_t* stack_pointer, const std::size_t stack_size)
    : state_(State::Ready)
    , pid_(pid_counter++)
    , stack_size_(stack_size)
    , stack_pointer_(stack_pointer)
{
    printf("Stack pointer %x\n", stack_pointer);
}

pid_t Process::pid() const 
{
    return pid_;
}

std::size_t Process::stack_size() const 
{
    return stack_size_;
}

std::size_t* Process::current_stack_pointer()
{
    return stack_pointer_;
}

const std::size_t* Process::current_stack_pointer() const 
{
    return stack_pointer_;
}

void Process::current_stack_pointer(std::size_t* stack_pointer)
{
    printf("Changing sp to: %p\n", stack_pointer);
    stack_pointer_ = stack_pointer;
}

} // namespace process     
} // namespace kernel     
} // namespace msos 

