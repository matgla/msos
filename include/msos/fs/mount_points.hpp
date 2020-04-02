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

#include <vector>
#include <string_view>

#include "msos/fs/i_filesystem.hpp"
#include "msos/fs/mount_point.hpp"

namespace msos
{
namespace fs
{

class MountPoints
{
public:
    const std::vector<MountPoint> get_mounted_points() const;
    const std::vector<MountPoint> get_mounted_points_under(const std::string_view& path) const;
    IFileSystem* get_mounted_filesystem(const std::string_view& path);
    bool mount_filesystem(const std::string_view& point, IFileSystem* filesystem);
    const MountPoint* get_mount_point(IFileSystem* filesystem) const;

private:
    std::vector<MountPoint> points_;
};

extern MountPoints mount_points;

} // namespace fs
} // namespace msos
