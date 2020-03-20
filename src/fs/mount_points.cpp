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

#include "msos/fs/mount_points.hpp"

#include <algorithm>

namespace msos
{
namespace fs
{

MountPoints mount_points;

IFileSystem* MountPoints::get_mounted_filesystem(const std::string_view& point)
{
    auto mount_point = std::find_if(points_.begin(), points_.end(), [point](const MountPoint& mp){
        return mp.point == point;
    });
    if (mount_point != points_.end())
    {
        return mount_point->filesystem;
    }
    return nullptr;
}

bool MountPoints::mount_filesystem(const std::string_view& point, IFileSystem* filesystem)
{
    points_.push_back(MountPoint(point, filesystem));
    return true;
}

} // namespace fs
} // namespace msos
