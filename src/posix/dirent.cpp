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
#include "msos/usart_printer.hpp"

extern "C"
{

struct DIRImpl
{
    uint32_t parent_namlen;
    char parent[100];
    msos::fs::IFileSystem* matched_filesystem;
    std::vector<msos::fs::MountPoint> mounted_points;
    bool first;
};

}

static UsartWriter writer;

void process_next_directory(std::string_view dir)
{
    writer << "Next: " << dir << endl;
}

void find_path(const char* path)
{
    // try to find
}

DIR* opendir(const char* dirname)
{
    writer << "Opening: " << dirname  << "." << endl;
    std::string_view path(dirname);

    while (path.length())
    {
        int slash_position = path.find("/");

        if (slash_position != 0 && slash_position != std::string_view::npos)
        {
            std::string_view before_slash = path.substr(0, slash_position);
            process_next_directory(before_slash);
        }
    }

    // DIR* dir = new DIR;
    // dir->ent.d_ino = 0;
    // dir->ent.d_off = 0;
    // dir->ent.d_reclen = 0;
    // dir->ent.d_namlen = 0;

    // dir->impl = new DIRImpl;


    // msos::fs::IFileSystem *fs = msos::fs::mount_points.get_mounted_filesystem(dirname);
    // // msos::fs::mount_points;
    // dir->impl->matched_filesystem = fs;

    // dir->impl->mounted_points = msos::fs::mount_points.get_mounted_points_under(dirname);
    // dir->impl->first = true;

    // if (std::string_view(dirname) == "/")
    // {
    //     dir->impl->parent_namlen = 1;
    //     dir->impl->parent[0] = '/';
    //     dir->impl->parent[1] = 0;
    // }
    // else
    // {
    //     if (!fs)
    //     {
    //         writer << "FS not found for file: " << dirname << endl;
    //     }
    //     const msos::fs::MountPoint* mp = msos::fs::mount_points.get_mount_point(fs);
    //     std::string_view path_in_fs(dirname);
    //     path_in_fs = path_in_fs.substr(mp->point.length(), path_in_fs.length());
    //     if (path_in_fs.length())
    //     {
    //         writer << "path is: " << path_in_fs << endl;

    //         dir->impl->parent_namlen = path_in_fs.length();
    //         std::memcpy(dir->impl->parent, path_in_fs.data(), path_in_fs.length());
    //     }
    //     else
    //     {
    //         writer << "path is /" << endl;

    //         dir->impl->parent_namlen = 1;
    //         dir->impl->parent[0] = '/';
    //         dir->impl->parent[1] = 0;
    //     }

    // }

    // return dir;
}

dirent* readdir(DIR *dirp)
{
    // if (dirp->impl->matched_filesystem)
    // {
    //     if (dirp->impl->first)
    //     {
    //         auto files = dirp->impl->matched_filesystem->list(dirp->impl->parent);
    //         dirp->impl->first = false;
    //         if (!files.empty())
    //         {
    //             const auto file = files.begin();
    //             std::memcpy(dirp->ent.d_name, (*file)->name().data(), (*file)->name().length());
    //             dirp->ent.d_namlen = (*file)->name().length();
    //             dirp->ent.d_name[dirp->ent.d_namlen] = 0;
    //             return &dirp->ent;
    //         }
    //     }

    //     bool previous_found = false;
    //     for (const auto& file : dirp->impl->matched_filesystem->list(dirp->impl->parent))
    //     {
    //         if (previous_found)
    //         {
    //             std::memcpy(dirp->ent.d_name, file->name().data(), file->name().length());
    //             dirp->ent.d_namlen = file->name().length();
    //             dirp->ent.d_name[dirp->ent.d_namlen] = 0;

    //             return &dirp->ent;
    //         }
    //         if (std::string_view(dirp->ent.d_name) == file->name())
    //         {
    //             previous_found = true;
    //         }
    //     }
    // }

    // if (dirp->impl->first)
    // {
    //     dirp->impl->first = false;
    //     if (!dirp->impl->mounted_points.empty())
    //     {
    //         const auto point = dirp->impl->mounted_points.begin();
    //         std::memcpy(dirp->ent.d_name, point->point.data(), point->point.length());
    //         dirp->ent.d_namlen = point->point.length();
    //         dirp->ent.d_name[dirp->ent.d_namlen] = 0;

    //         return &dirp->ent;
    //     }
    // }

    // bool previous_found = false;
    // for (auto& mp : dirp->impl->mounted_points)
    // {
    //     if (previous_found)
    //     {
    //         std::memcpy(dirp->ent.d_name, mp.point.data(), mp.point.length());
    //         dirp->ent.d_namlen = mp.point.length();
    //         dirp->ent.d_name[dirp->ent.d_namlen] = 0;

    //         return &dirp->ent;
    //     }
    //     if (std::string_view(dirp->ent.d_name) == mp.point)
    //     {
    //         previous_found = true;
    //     }
    // }
    return nullptr;
}

int closedir(DIR *dirp)
{
    // delete dirp->impl;
    // delete dirp;
    return 0;
}
