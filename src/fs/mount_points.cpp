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

const MountPoint* MountPoints::get_best_mount_point(const eul::filesystem::path& path) const
{
    std::string_view best_mount_point;
    for (auto& point : points_)
    {
        std::size_t index = path.native().find(point.point.native());
        if (index != std::string_view::npos)
        {
            if (point.point.native().size() > best_mount_point.size())
            {
                best_mount_point = point.point.native();
            }
        }
    }

    auto mount_point = std::find_if(points_.begin(), points_.end(), [best_mount_point](const MountPoint& mp){
        return mp.point.native() == best_mount_point;
    });
    if (mount_point != points_.end())
    {
        return &(*mount_point);
    }

    return nullptr;

}

bool MountPoints::mount_filesystem(const eul::filesystem::path& path, IFileSystem* filesystem)
{
    points_.push_back(MountPoint(path, filesystem));
    return true;
}

} // namespace fs
} // namespace msos
