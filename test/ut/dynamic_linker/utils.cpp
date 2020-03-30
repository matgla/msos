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

#include "test/ut/dynamic_linker/utils.hpp"

#include <exception>

#include <iostream>
#include <fstream>

namespace test
{
namespace ut
{
namespace dynamic_linker
{

std::vector<uint8_t> load_test_binary(const std::string_view& path)
{
    std::ifstream binary_loader;
    binary_loader.open(path.data(), std::ifstream::in | std::ifstream::binary);
    std::vector<uint8_t> data;
    if (!binary_loader.is_open())
    {
        throw std::runtime_error("Can't open test_binary.bin file");
    }
    while (binary_loader.good())
    {
        data.push_back(binary_loader.get());
    }

    return data;
}

} // namespace dynamic_linker
} // namespace ut
} // namespace test

