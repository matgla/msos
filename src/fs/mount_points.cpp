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

const std::vector<MountPoint> MountPoints::get_mounted_points() const
{
    return points_;
}

const std::vector<MountPoint> MountPoints::get_mounted_points_under(const std::string_view& path) const
{
    std::vector<MountPoint> matched;

    std::copy_if(points_.begin(), points_.end(), std::back_inserter(matched),
        [path](const MountPoint& point) {
            if (point.point.find(path) == 0)
            {
                std::string_view trimmed_path = point.point.substr(path.length(), point.point.length());
                std::size_t last_slash = trimmed_path.find_last_of("/");

                if (last_slash == 0 || last_slash == std::string_view::npos)
                {
                    return true;
                }
            }
            return false;
        });
    return matched;
}

const MountPoint* MountPoints::get_best_mount_point(std::string_view path) const
{
    std::string_view best_mount_point;
    std::string_view path_to_executable(path);
    for (auto& point : points_)
    {
        std::size_t index = path_to_executable.find(point.point);
        if (index != std::string_view::npos)
        {
            if (point.point.size() > best_mount_point.size())
            {
                best_mount_point = point.point;
            }
        }
    }

    if (best_mount_point != "/")
    {
        path_to_executable.remove_prefix(best_mount_point.size());
    }

    auto mount_point = std::find_if(points_.begin(), points_.end(), [best_mount_point](const MountPoint& mp){
        return mp.point == best_mount_point;
    });
    if (mount_point != points_.end())
    {
        return &(*mount_point);
    }

    return nullptr;

}

IFileSystem* MountPoints::get_mounted_filesystem(const std::string_view& path)
{
    std::string_view best_mount_point;
    std::string_view path_to_executable(path);
    for (auto& point : points_)
    {
        std::size_t index = path_to_executable.find(point.point);
        if (index != std::string_view::npos)
        {
            if (point.point.size() > best_mount_point.size())
            {
                best_mount_point = point.point;
            }
        }
    }

    if (best_mount_point != "/")
    {
        path_to_executable.remove_prefix(best_mount_point.size());
    }

    auto mount_point = std::find_if(points_.begin(), points_.end(), [best_mount_point](const MountPoint& mp){
        return mp.point == best_mount_point;
    });
    if (mount_point != points_.end())
    {
        return mount_point->filesystem;
    }

    return nullptr;
}

bool MountPoints::mount_filesystem(std::string_view point, IFileSystem* filesystem)
{
    std::size_t dirname_start = point.find_first_not_of("/");
    if (dirname_start != std::string_view::npos)
    {
        point = point.substr(dirname_start, point.length());
    }
    else if(!point.empty())
    {
        point = {};
    }


    points_.push_back(MountPoint(point, filesystem));
    return true;
}

const MountPoint* MountPoints::get_mount_point(IFileSystem* filesystem) const
{
    auto mount_point = std::find_if(points_.begin(), points_.end(), [filesystem](const MountPoint& mp){
        return mp.filesystem == filesystem;
    });

    if (mount_point != points_.end())
    {
        return &(*mount_point);
    }

    return nullptr;
}

const MountPoint* MountPoints::get_mount_point(std::string_view point) const
{
    std::size_t dirname_start = point.find_first_not_of("/");
    if (dirname_start != std::string_view::npos)
    {
        point = point.substr(dirname_start, point.length());
    }
    else if(!point.empty())
    {
        point = {};
    }

    auto mount_point = std::find_if(points_.begin(), points_.end(), [point](const MountPoint& mp){
        return mp.point == point;
    });

    if (mount_point != points_.end())
    {
        return &(*mount_point);
    }

    return nullptr;
}

} // namespace fs
} // namespace msos
