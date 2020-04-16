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

#include "msos/drivers/i_driver.hpp"

namespace msos
{
namespace drivers
{
namespace character
{

struct CharacterDriver : public IDriver
{
public:
    using DataType = gsl::span<uint8_t>;
    using ConstDataType = gsl::span<const uint8_t>;

    virtual ssize_t read(DataType data) = 0;
    virtual ssize_t write(const ConstDataType data) = 0;
    virtual void close() = 0;
    virtual ssize_t size() = 0;
};

} // namespace character
} // namespace drivers
} // namespace msos
