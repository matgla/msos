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

#include "msos/kernel/process/process.hpp"

#include <fcntl.h>

#include "msos/drivers/device_fs.hpp"

namespace msos
{
namespace kernel
{
namespace process
{

Process::Process(State state, pid_t pid)
    : fd_map_(0x7)
    , pid_(pid)
    , state_(state)
{
    auto* tty = drivers::DeviceFs::get_instance().get_driver("tty1");
    if (tty)
    {
        fd_[0] = tty->file("tty1", O_RDWR);
        fd_[1] = tty->file("tty1", O_RDWR);
        fd_[2] = tty->file("tty1", O_RDWR);
    }
}

Process::Process(const Process& other)
    : fd_map_(other.fd_map_)
    , pid_(other.pid_)
    , state_(other.state_)
{
    for (int i = 0; i < 8; i++)
    {
        fd_[i] = std::move(other.fd_[i]->clone());
    }
}

pid_t Process::pid() const
{
    return pid_;
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
            printf("FD to add: %d\n", i);
            fd_[i] = std::move(file);
            return i;
        }
    }
    return -1;
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

msos::fs::IFile* Process::get_file(int fd) const
{

    if (fd_map_ & (1 << fd))
    {
        return fd_[fd].get();
    }
    return nullptr;
}

std::string_view Process::get_cwd() const
{
    return cwd_;
}


} // namespace process
} // namespace kernel
} // namespace msos

