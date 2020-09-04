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

#include "msos/fs/ramfs_data.hpp"

namespace msos
{
namespace fs
{

RamFsData RamFsData::create_file(const eul::filesystem::path& name, const std::vector<uint8_t>& data)
{
    return RamFsData(name, data);
}

RamFsData RamFsData::create_directory(const eul::filesystem::path& name)
{
    return RamFsData(name);
}

RamFsData::RamFsData(const eul::filesystem::path& name)
    : filename_(name.native())
    , data_{}
    , is_directory_(true)
{
}

RamFsData::RamFsData(const eul::filesystem::path& name, const std::vector<uint8_t>& data)
    : filename_{name}
    , data_{data}
    , is_directory_(false)
{
}

const std::string& RamFsData::filename() const
{
    return filename_.native();
}

std::vector<uint8_t>& RamFsData::data()
{
    return data_;
}

bool RamFsData::is_directory() const
{
    return is_directory_;
}

} // namespace fs
} // namespace msos
