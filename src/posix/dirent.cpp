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

#include "msos/posix/dirent.h"

#include <vector>
#include <cstring>

#include "msos/fs/mount_points.hpp"

extern "C"
{

struct DIRImpl
{
    msos::fs::IFileSystem* matched_filesystem;
    std::vector<msos::fs::MountPoint> mounted_points;
    bool first;
};

}


DIR* opendir(const char* dirname)
{
    DIR* dir = new DIR;
    dir->ent.d_ino = 0;
    dir->ent.d_off = 0;
    dir->ent.d_reclen = 0;
    dir->ent.d_namlen = 0;

    dir->impl = new DIRImpl;

    if (std::string_view(dirname) == "/")
    {
        dir->ent.d_namlen = 1;
        dir->ent.d_name[0] = '/';
        dir->ent.d_name[1] = 0;
    }

    msos::fs::IFileSystem *fs = msos::fs::mount_points.get_mounted_filesystem(dirname);
    // msos::fs::mount_points;
    dir->impl->matched_filesystem = fs;

    dir->impl->mounted_points = msos::fs::mount_points.get_mounted_points_under(dirname);
    dir->impl->first = true;
    return dir;
}

dirent* readdir(DIR *dirp)
{
    // UsartWriter writer{};
    // writer << "Opened file: " << dirp->ent.d_name << endl;
    // for (auto& mp : dirp->impl->mounted_points)
    // {
    //     writer << "Processing mp" << endl;
    //     writer << mp.point << endl;
    // }
    if (dirp->impl->matched_filesystem)
    {
        // process next file
    }

    if (dirp->impl->first)
    {
        dirp->impl->first = false;
        if (!dirp->impl->mounted_points.empty())
        {
            const auto point = dirp->impl->mounted_points.begin();
            std::memcpy(dirp->ent.d_name, point->point.data(), point->point.length());
            dirp->ent.d_namlen = point->point.length();
            return &dirp->ent;
        }
    }

    bool previous_found = false;
    for (auto& mp : dirp->impl->mounted_points)
    {
        if (previous_found)
        {
            std::memcpy(dirp->ent.d_name, mp.point.data(), mp.point.length());
            dirp->ent.d_namlen = mp.point.length();
            return &dirp->ent;
        }
        if (std::string_view(dirp->ent.d_name) == mp.point)
        {
            previous_found = true;
        }
    }
    return nullptr;
}

int closedir(DIR *dirp)
{
    delete dirp->impl;
    delete dirp;
    return 0;
}
