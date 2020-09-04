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
#include <vector>

#include <eul/filesystem/path.hpp>

namespace msos
{
namespace fs
{

class RamFsData
{
public:
    static RamFsData create_file(const eul::filesystem::path& name, const std::vector<uint8_t>& data);
    static RamFsData create_directory(const eul::filesystem::path& name);
    const std::string& filename() const;

    std::vector<uint8_t>& data();

    bool is_directory() const;

private:
    RamFsData(const eul::filesystem::path& name);
    RamFsData(const eul::filesystem::path& name, const std::vector<uint8_t>& data);

    eul::filesystem::path filename_;
    std::vector<uint8_t> data_;
    bool is_directory_;
};

} // namespace fs
} // namespace msos
