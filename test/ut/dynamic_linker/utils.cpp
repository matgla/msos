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
    std::cerr << "AAA: " << path << std::endl;
    std::string path_in_system = "./";
    path_in_system += std::filesystem::path(parent_path).parent_path();
    std::cerr << path_in_system << std::endl;
    path_in_system += "/";
    path_in_system += std::string(path.data());
    std::cerr << "Opening: " << path_in_system << std::endl;
    binary_loader.open(path_in_system, std::ifstream::in | std::ifstream::binary);
    std::vector<uint8_t> data;
    if (!binary_loader.is_open())
    {
        std::cerr << "Throw" << std::endl;
        std::string err = "Can't open: ";
        err += path_in_system;
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

