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

#include "msos/kernel/process/process.hpp"

#include <algorithm>

#include <thread>
#include <iostream>

#include <unistd.h>

namespace msos
{
namespace kernel
{
namespace process
{

static pid_t pid_counter = 1;

Process::Process(const Process& process)
    : state_(process.state_)
    , pid_(process.pid_)
    , stack_size_(process.stack_size_)
    , fd_map_(process.fd_map_)
    , locks_{}
{
}

std::vector<std::thread> threads_;

Process::Process(const std::size_t process_entry, const std::size_t stack_size, std::size_t arg)
    : state_(State::Ready)
    , pid_(pid_counter++)
    , stack_size_(stack_size)
    , stack_(new std::size_t[stack_size/(sizeof(std::size_t))]())
    , fd_map_(0x7)
    , locks_{}
{
    std::cout << "Spawing process: " << std::hex << process_entry << std::endl;
    threads_.emplace_back(reinterpret_cast<void(*)(void*)>(process_entry), nullptr);

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

void Process::print() const
{
}

void Process::block(void* semaphore)
{
    auto lock = std::find(locks_.begin(), locks_.end(), reinterpret_cast<std::size_t>(semaphore));
    if (lock == locks_.end())
    {
        locks_.push_back(reinterpret_cast<std::size_t>(semaphore));
    }
    state_ = State::Blocked;
}

void Process::unblock(void* semaphore)
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

Process::State Process::get_state() const
{
    return state_;
}

int Process::add_file(std::unique_ptr<msos::fs::IFile>&& file)
{
    for (int i = 0; i < 8; i++)
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

bool Process::validate_stack() const
{
    return stack_[0] == 0xdeadbeef;
}



} // namespace process
} // namespace kernel
} // namespace msos

