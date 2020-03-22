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
#include <utility>

#include "msos/usart_printer.hpp"

#include "msos/kernel/process/process.hpp"
#include "msos/kernel/process/registers.hpp"
#include "msos/syscalls/syscalls.hpp"

namespace msos
{
namespace kernel
{
namespace process
{

void exit_handler()
{
    printf("Process exited\n");
    trigger_syscall(SyscallNumber::SYSCALL_EXIT, NULL, NULL);

    while(1);
}

constexpr uint32_t default_psr_status = 0x21000000;

void print_stack(const uint32_t* stack, std::size_t length)
{
    for (std::size_t i = 0; i < length / 4; i+=4)
    {
        printf("%p: 0x%08x 0x%08x 0x%08x 0x%08x\n", &stack[i], stack[i], stack[i+1], stack[i+2], stack[i+3]);
    }
}

uint32_t Process::patch_register(const Process& parent, uint32_t reg)
{
    uint32_t parent_stack = reinterpret_cast<uint32_t>(parent.stack_.get());
    printf("Compare %x >= %x && < %x\n", reg, parent_stack, parent_stack + parent.stack_size_);

    if (reg >= parent_stack - parent.stack_size_ && reg < parent_stack)
    {
        uint32_t stack = reinterpret_cast<uint32_t>(stack_.get());
        printf("Patching register from 0x%x to 0x%x\n", reg, (stack + (reg - parent_stack)));
        return stack + (reg - parent_stack);
    }
    return reg;
}

static pid_t pid_counter = 1;

Process::Process(const Process& process)
    : state_(process.state_)
    , pid_(process.pid_)
    , stack_size_(process.stack_size_)
    , fd_map_(process.fd_map_)
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
    for (int i = 0; i < 8; i++)
    {
        fd_[i] = std::move(process.fd_[i]->clone());
    }
}

Process::Process(const std::size_t process_entry, const std::size_t stack_size, uint32_t arg)
    : state_(State::Ready)
    , pid_(pid_counter++)
    , stack_size_(stack_size)
    , stack_(new std::size_t[stack_size/(sizeof(std::size_t))]())
    , fd_map_(0x7)
{
    uint8_t* stack_ptr = reinterpret_cast<uint8_t*>(stack_.get()) + stack_size_ - sizeof(HardwareStoredRegisters) - sizeof(SoftwareStoredRegisters);
    std::size_t required_stack_size = sizeof(HardwareStoredRegisters) + sizeof(SoftwareStoredRegisters);
    if (required_stack_size >= stack_size_)
    {
        printf ("Not enough space on stack in child task, required %d bytes.\n", required_stack_size);
    }

    HardwareStoredRegisters* hw_registers = reinterpret_cast<HardwareStoredRegisters*>(stack_ptr + sizeof(SoftwareStoredRegisters));
    hw_registers->r0 = arg;
    hw_registers->r1 = 0;
    hw_registers->r2 = 0;
    hw_registers->r3 = 0;

    SoftwareStoredRegisters* sw_registers = reinterpret_cast<SoftwareStoredRegisters*>(stack_ptr);

    sw_registers->r4 = 0;
    sw_registers->r5 = 0;
    sw_registers->r6 = 0;
    sw_registers->r7 = 0;
    sw_registers->r8 = 0;
    sw_registers->r9 = 0;
    sw_registers->r10 = 0;
    sw_registers->r11 = 0;

    //uint32_t psp = get_psp();
    hw_registers->psr = default_psr_status;
    hw_registers->lr = reinterpret_cast<uint32_t>(&exit_handler);
    hw_registers->pc = process_entry;
    sw_registers->lr = return_to_thread_mode_psp;
    current_stack_pointer_ = reinterpret_cast<std::size_t*>(stack_ptr);
    printf("Process created\n");
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

void Process::block()
{
    UsartWriter{} << "State set to blocked: " << pid_ << hex << ", 0x" <<  reinterpret_cast<uint32_t>(this) << endl;
    state_ = State::Blocked;
}

void Process::unblock()
{
    if (state_ == State::Blocked)
    {
        state_ = State::Ready;
    }
}

Process::State Process::get_state() const
{
    return state_;
}

int Process::add_file(std::unique_ptr<msos::fs::IFile>&& file)
{
    int i = 0;

    for (i = 0; i < 8; i++)
    {
        if ((fd_map_ & (1 << i)) == 0)
        {
            fd_map_ |= (1 << i);
            fd_[i] = std::move(file);
            return i;
        }
    }
    return -1;
}

msos::fs::IFile* Process::get_file(int fd) const
{
    if (fd_map_ & (1 << fd))
    {
        return fd_[fd].get();
    }
    return nullptr;
}

int Process::remove_file(int fd)
{
    if (fd_map_ & (1 << fd))
    {
        fd_map_ &= ~(1 << fd);
        fd_[fd].reset();
        return 0;
    }
    return -1;
}


} // namespace process
} // namespace kernel
} // namespace msos

