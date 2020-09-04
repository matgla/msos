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

#include <unistd.h>

#include "msos/fs/fwd.hpp"

#include "msos/limits.hpp"

namespace msos
{
namespace kernel
{
namespace process
{

class Process
{
public:
    virtual ~Process() = default;

    enum class State : uint8_t
    {
        Ready,
        Blocked,
        Running,
        Killed
    };

    Process(State state, pid_t pid);
    Process(const Process& other);

    virtual void block(void* semaphore) = 0;
    virtual void unblock(void* semaphore) = 0;

    virtual const std::size_t* current_stack_pointer() const = 0;
    virtual void current_stack_pointer(const std::size_t* stack_pointer) = 0;

    pid_t pid() const;
    State get_state() const;
    int add_file(std::unique_ptr<msos::fs::IFile>&& file);
    int remove_file(int fd);
    msos::fs::IFile* get_file(int fd) const;

    std::string_view get_cwd() const;
    bool set_cwd(std::string_view path);

protected:
    int fd_map_;
    pid_t pid_;
    State state_;
    std::array<std::unique_ptr<msos::fs::IFile>, 8> fd_;
    char cwd_[PATH_MAX] = "/";
};

} // namespace process
} // namespace kernel
} // namespace msos

