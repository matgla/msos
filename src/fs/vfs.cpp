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

#include "msos/fs/vfs.hpp"

#include <algorithm>
#include <climits>
#include <filesystem>

#include "msos/posix/dirent_utils.hpp"

#include <eul/utils/unused.hpp>

#include "msos/usart_printer.hpp"

namespace msos
{
namespace fs
{

static UsartWriter writer;

int Vfs::mount(drivers::storage::BlockDevice& device)
{
    UNUSED1(device);
    return 1;
}

int Vfs::umount()
{
    return 1;
}

int Vfs::create()
{
    return 1;
}

/* jeśli get to best match jest na przykład /rom -> ramfs
   jeśli list to best match jest juz rom, bo chodzi o katalog decolowy
   */

int Vfs::mkdir(std::string_view path, int mode)
{
    const MountPoint* mp = mount_points_.get_best_mount_point(path);
    std::string_view path_in_fs = path.substr(path.find(mp->point) + mp->point.length(), path.length());
    return mp->filesystem->mkdir(path_in_fs, mode);
}

int Vfs::remove(std::string_view path)
{
    const MountPoint* mp = mount_points_.get_best_mount_point(path);
    std::string_view path_in_fs = path.substr(path.find(mp->point) + mp->point.length(), path.length());
    return mp->filesystem->remove(path_in_fs);
}

int Vfs::stat(std::string_view path)
{
    const MountPoint* mp = mount_points_.get_best_mount_point(path);
    std::string_view path_in_fs = path.substr(path.find(mp->point) + mp->point.length(), path.length());
    return mp->filesystem->stat(path_in_fs);
}

std::unique_ptr<IFile> Vfs::get(std::string_view path)
{
    const MountPoint* mp = mount_points_.get_best_mount_point(path);
    std::string_view path_in_fs = path.substr(path.find(mp->point) + mp->point.length(), path.length());
    return mp->filesystem->get(path_in_fs);
}

std::unique_ptr<IFile> Vfs::create(std::string_view path)
{
    const MountPoint* mp = mount_points_.get_best_mount_point(path);
    std::string_view path_in_fs = path.substr(path.find(mp->point) + mp->point.length(), path.length());
    return mp->filesystem->create(path_in_fs);
}


std::vector<std::unique_ptr<IFile>> Vfs::list(std::string_view path)
{
    std::string destination(path);
    const MountPoint* mp = mount_points_.get_best_mount_point(path);
    std::string_view path_in_fs = path.substr(path.find(mp->point) + mp->point.length(), path.length());
    return mp->filesystem->list(path_in_fs);
}

void Vfs::mount_fs(std::string_view path, IFileSystem* fs)
{
    mount_points_.mount_filesystem(path, fs);
}


} // namespace fs
} // namespace msos
