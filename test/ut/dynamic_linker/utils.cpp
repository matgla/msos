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

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstring>

extern const char* parent_path;

namespace test
{
namespace ut
{
namespace dynamic_linker
{

std::vector<uint8_t> load_test_binary(const std::string& path)
{
    std::ifstream binary_loader;
    std::vector<uint8_t> data;
    std::string binary_path = parent_path;
    binary_path = binary_path.substr(0, binary_path.find_last_of("/") + 1);
    binary_path += path;
    binary_loader.open(binary_path, std::ifstream::in | std::ifstream::binary);
    if (!binary_loader.is_open())
    {
        std::cerr << "Throw" << std::endl;
        std::string err = "Can't open: ";
        err += std::filesystem::absolute(binary_path);
        throw std::runtime_error(err);
    }
    while (binary_loader.good())
    {
        data.push_back(binary_loader.get());
    }

    binary_loader.close();

    return data;
}

} // namespace dynamic_linker
} // namespace ut
} // namespace test

