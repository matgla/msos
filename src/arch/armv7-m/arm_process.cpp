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

#include "arch/armv7-m/arm_process.hpp"

#include <cstring>
#include <utility>

#include <fcntl.h>

#include "msos/usart_printer.hpp"

#include "arch/armv7-m/registers.hpp"
#include "msos/syscalls/syscalls.hpp"
#include "msos/drivers/device_fs.hpp"

namespace msos
{
namespace arch
{
namespace armv7m
{

void exit_handler()
{
    trigger_syscall(SyscallNumber::SYSCALL_EXIT, NULL, NULL);

    while(1);
}

constexpr uint32_t default_psr_status = 0x21000000;

static pid_t pid_counter = 1;

static UsartWriter writer;

ArmProcess::ArmProcess(const ArmProcess& process)
    : Process(process)
    , stack_size_(process.stack_size_)
    , locks_{}
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

ArmProcess::ArmProcess(const std::size_t process_entry, const std::size_t stack_size, std::size_t arg)
    : Process(State::Ready, pid_counter++)
    , stack_size_(stack_size)
    , stack_(new std::size_t[stack_size/(sizeof(std::size_t))]())
    , locks_{}
{
    stack_[0] = 0xdeadbeef;
    uint8_t* stack_ptr = reinterpret_cast<uint8_t*>(stack_.get()) + stack_size_ - sizeof(HardwareStoredRegisters) - sizeof(SoftwareStoredRegisters);

    std::size_t required_stack_size = sizeof(HardwareStoredRegisters) + sizeof(SoftwareStoredRegisters);
    if (required_stack_size >= stack_size_)
    {
        // assert
    }

    HardwareStoredRegisters* hw_registers = reinterpret_cast<HardwareStoredRegisters*>(stack_ptr + sizeof(SoftwareStoredRegisters));
    hw_registers->r0 = reinterpret_cast<std::size_t>(arg);
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
    hw_registers->lr = reinterpret_cast<std::size_t>(&exit_handler);
    hw_registers->pc = process_entry;
    sw_registers->lr = return_to_thread_mode_psp;
    current_stack_pointer_ = reinterpret_cast<std::size_t*>(stack_ptr);
}

const std::size_t* ArmProcess::stack_pointer() const
{
    return stack_.get();
}

std::size_t ArmProcess::stack_usage() const
{
    return reinterpret_cast<const uint8_t*>(stack_.get()) + stack_size_ - reinterpret_cast<const uint8_t*>(current_stack_pointer_);
}

std::size_t ArmProcess::stack_size() const
{
    return stack_size_;
}

const std::size_t* ArmProcess::current_stack_pointer()
{
    if (!validate_stack())
    {
        // TODO panic
        return nullptr;
    }
    return current_stack_pointer_;
}

const std::size_t* ArmProcess::current_stack_pointer() const
{
    if (!validate_stack())
    {
        // TODO panic
        return nullptr;
    }
    return current_stack_pointer_;
}

void ArmProcess::current_stack_pointer(const std::size_t* stack_pointer)
{
    current_stack_pointer_ = stack_pointer;
}

void ArmProcess::block(void* semaphore)
{
    auto lock = std::find(locks_.begin(), locks_.end(), reinterpret_cast<std::size_t>(semaphore));
    if (lock == locks_.end())
    {
        locks_.push_back(reinterpret_cast<std::size_t>(semaphore));
    }
    state_ = State::Blocked;
}

void ArmProcess::unblock(void* semaphore)
{
    auto lock = std::find(locks_.begin(), locks_.end(), reinterpret_cast<std::size_t>(semaphore));
    if (lock != locks_.end())
    {
        locks_.erase(lock);

        if (locks_.empty())
        {
            state_ = State::Ready;
        }
    }
}

bool ArmProcess::validate_stack() const
{
    return stack_[0] == 0xdeadbeef;
}



} // namespace archv7m
} // namespace arch
} // namespace msos

