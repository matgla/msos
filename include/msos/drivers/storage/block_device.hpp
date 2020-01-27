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

#pragma once

#include <cstdint>
#include <string_view>

#include <gsl/span>

namespace msos 
{
namespace drivers 
{
namespace storage 
{

class BlockDevice
{
public:
    using StreamType = gsl::span<uint8_t>;

    BlockDevice(const std::size_t size, const std::size_t read_size, const std::size_t write_size, const std::size_t erase_size);

    virtual ~BlockDevice() = default;

    virtual int init() = 0;
    virtual int deinit() = 0;
    virtual int sync();

    std::size_t read_size() const;
    std::size_t write_size() const;
    std::size_t erase_size() const;
    virtual std::size_t erase_size_at_address(std::size_t address) const;


    std::size_t size() const;

    virtual int read(std::size_t address, StreamType& stream) const;
    virtual int write(std::size_t address, const StreamType& stream);
    virtual int erase(std::size_t address, std::size_t size);

    virtual std::string_view name() const = 0;

protected:
    bool is_write_valid(std::size_t address, const StreamType& stream) const;
    bool is_read_valid(std::size_t address, StreamType& stream) const;
    bool is_erase_valid(std::size_t address, std::size_t size) const;

    virtual int perform_read(std::size_t address, StreamType& stream) const = 0;
    virtual int perform_write(std::size_t address, const StreamType& stream) = 0;
    virtual int perform_erase(std::size_t address, std::size_t size) = 0;

    std::size_t size_;
    std::size_t read_size_;
    std::size_t write_size_;
    std::size_t erase_size_;
};

} // namespace storage 
} // namespace drivers
} // namespace msos 

