// This file is part of MSOS project.
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

#include "msos/drivers/storage/ram_block_device.hpp"

#include <cstring>

#include <eul/utils/unused.hpp>

#include "msos/kernel/synchronization/atomic.hpp"

namespace msos
{
namespace drivers
{
namespace storage
{

int RamBlockDevice::init()
{
    uint32_t ref_counter = kernel::atomic::increment<uint32_t>(reference_counter_, 1);

    if (ref_counter != 1)
    {
        return 0;
    }

    if (!memory_)
    {
        memory_.reset(new uint8_t[size_]);
    }

    if (!memory_)
    {
        return -1;
    }

    std::memset(memory_.get(), 0, size_);
    was_initialized_ = true;
    return 0;
}

int RamBlockDevice::deinit()
{
    if (!was_initialized_)
    {
        return 0;
    }

    uint32_t ref_counter = kernel::atomic::decrement<uint32_t>(reference_counter_, 1);

    if (ref_counter != 0)
    {
        return 0;
    }

    was_initialized_ = false;
    return 0;
}

std::string_view RamBlockDevice::name() const
{
    return "RAM_BD";
}

int RamBlockDevice::perform_read(std::size_t address, StreamType& stream) const
{
    if (!was_initialized_)
    {
        return -1;
    }

    std::memcpy(stream.data(), memory_.get() + address, stream.size());

    return 1;
}

int RamBlockDevice::perform_write(std::size_t address, const StreamType& stream)
{
    if (!was_initialized_)
    {
        return -1;
    }

    std::memcpy(memory_.get() + address, stream.data(), stream.size());
    return 0;
}

int RamBlockDevice::perform_erase(std::size_t address, std::size_t size)
{
    UNUSED2(address, size);
    if (!was_initialized_)
    {
        return -1;
    }

    return 0;
}

} // namespace storage
} // namespace drivers
} // namespace msos

