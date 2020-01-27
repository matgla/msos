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

#include "msos/drivers/storage/block_device.hpp" 

#include <cstdint>
#include <memory>

namespace msos
{
namespace drivers
{
namespace storage
{

class RamBlockDevice : public BlockDevice 
{
public:
    using BlockDevice::BlockDevice; 

    int init() override;
    int deinit() override;  

    std::string_view name() const override;

protected:

    int perform_read(std::size_t address, StreamType& stream) const override;
    int perform_write(std::size_t address, const StreamType& stream) override;
    int perform_erase(std::size_t address, std::size_t size) override;

private:
    bool was_initialized_ = false;
    uint32_t reference_counter_ = 0;
    std::unique_ptr<uint8_t[]> memory_;
};

} // namespace storage     
} // namespace drivers     
} // namespace msos 

