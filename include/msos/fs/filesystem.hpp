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

#include <string_view>

#include "msfs/block_device.hpp"

namespace msfs
{

class FileSystem
{
public:
    virtual ~FileSystem() = default;

    virtual int mount(BlockDevice& device) = 0;
    virtual int umount() = 0;

    virtual int create() = 0;
    virtual int mkdir(const std::string_view path, int mode) = 0;

    virtual int remove(const std::string_view path) = 0;
    virtual int stat(const std::string_view path) = 0;

protected:
    static bool mounted_;
};

} // namespace msfs

