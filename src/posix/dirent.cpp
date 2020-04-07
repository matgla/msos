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

#include <eul/filesystem/path.hpp>

#include "msos/fs/vfs.hpp"
#include "msos/posix/dirent_utils.hpp"

extern "C"
{

struct DIRImpl
{
    char* path;
    msos::fs::IFileSystem* filesystem;
};

}

DIR* opendir(const char* dirname)
{
    eul::filesystem::path path(dirname);
    path = path.lexically_normal();

    auto& vfs = msos::fs::Vfs::instance();
    auto file = vfs.get(path.native());
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
    dir->impl->filesystem = &vfs;

    dir->impl->path = new char[path.native().length() + 1];
    std::memcpy(dir->impl->path, path.c_str(), path.native().length());
    dir->impl->path[path.native().length()] = 0;

    return dir;
}

dirent* readdir(DIR *dirp)
{
    // writer << "Read dir" << endl;
    auto files = dirp->impl->filesystem->list(dirp->impl->path);
    if (files.empty())
    {
        return nullptr;
    }

    if (dirp->ent.d_namlen == 0)
    {
        // writer << "Namelen is 0" << endl;
        const auto& file = *files.front();
        // writer << "size: " << files.size() << ", " << files.front()->name() << endl;

        if (file.name().empty())
        {
            dirp->ent.d_name[0] = '/';
            dirp->ent.d_name[1] = 0;
            dirp->ent.d_namlen = 1;
            return &dirp->ent;
        }
        dirp->ent.d_namlen = file.name().length();
        std::memcpy(dirp->ent.d_name, file.name().data(), file.name().length());
        dirp->ent.d_name[dirp->ent.d_namlen] = 0;
        return &dirp->ent;
    }

    bool found_previous = false;
    std::string_view previous = dirp->ent.d_name;
    if (previous == "/")
    {
        previous = {};
    }

    for (const auto& file : files)
    {
        if (found_previous)
        {
            std::memcpy(dirp->ent.d_name, file->name().data(), file->name().length());
            dirp->ent.d_namlen = file->name().length();
            dirp->ent.d_name[dirp->ent.d_namlen] = 0;
            return &dirp->ent;
        }
        // writer << file->name() << "==" << dirp->ent.d_name << endl;
        if (file->name() == previous)
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
