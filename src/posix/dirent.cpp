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
#include "msos/posix/dirent_utils.hpp"

extern "C"
{

struct DIRImpl
{
    char* path;
    msos::fs::IFileSystem* filesystem;
};

}

// static UsartWriter writer;

void process_next_directory(std::string_view dir)
{
    // writer << "Next: " << dir << endl;
}

void find_path(const char* path)
{
    // try to find
}

DIR* opendir(const char* dirname)
{
    const msos::fs::MountPoint* root = msos::fs::mount_points.get_mount_point("/");
    if (root == nullptr)
    {
        return nullptr;
    }

    std::string_view path(dirname);

    auto file = root->filesystem->get(path);
    if (!file)
    {
        return nullptr; // no such directory
    }

    DIR* dir = new DIR;
    dir->ent.d_ino = 0;
    dir->ent.d_off = 0;
    dir->ent.d_reclen = 0;
    dir->ent.d_namlen = 0;

    dir->impl = new DIRImpl;
    dir->impl->filesystem = root->filesystem;

    dir->impl->path = new char[path.length() + 1];
    std::memcpy(dir->impl->path, path.data(), path.length());
    dir->impl->path[path.length()] = 0;

    return dir;
}

dirent* readdir(DIR *dirp)
{
    auto files = dirp->impl->filesystem->list(dirp->impl->path);
    if (files.empty())
    {
        return nullptr;
    }

    if (dirp->ent.d_namlen == 0)
    {
        const auto& file = *files.front();
        std::memcpy(dirp->ent.d_name, file.name().data(), file.name().length());
        dirp->ent.d_namlen = file.name().length();
        dirp->ent.d_name[dirp->ent.d_namlen] = 0;
        return &dirp->ent;
    }

    bool found_previous = false;
    for (const auto& file : files)
    {
        if (found_previous)
        {
            std::memcpy(dirp->ent.d_name, file->name().data(), file->name().length());
            dirp->ent.d_namlen = file->name().length();
            dirp->ent.d_name[dirp->ent.d_namlen] = 0;
            return &dirp->ent;
        }
        if (file->name() == std::string_view(dirp->ent.d_name))
        {
            found_previous = true;
        }
    }

    return nullptr;
}

int closedir(DIR *dirp)
{
    delete[] dirp->impl->path;
    delete dirp->impl;
    delete dirp;
    return 0;
}
