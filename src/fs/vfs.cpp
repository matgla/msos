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

#include <eul/utils/unused.hpp>

#include "msos/posix/dirent_utils.hpp"
#include "msos/usart_printer.hpp"

namespace msos
{
namespace fs
{

static UsartWriter writer;

Vfs& Vfs::instance()
{
    static Vfs v;
    return v;
}

int Vfs::mount(drivers::storage::BlockDevice& device)
{
    UNUSED1(device);
    return 1;
}

int Vfs::umount()
{
    return 1;
}

int Vfs::create(int flags)
{
    UNUSED1(flags);
    return 1;
}

int Vfs::mkdir(const eul::filesystem::path& path, int mode)
{
    const MountPoint* mp = mount_points_.get_best_mount_point(path);
    return mp == nullptr ? -1 : mp->filesystem->mkdir(path.lexically_relative(mp->point), mode);
}

int Vfs::remove(const eul::filesystem::path& path)
{
    const MountPoint* mp = mount_points_.get_best_mount_point(path);
    return mp == nullptr ? -1 : mp->filesystem->remove(path.lexically_relative(mp->point));
}

int Vfs::stat(const eul::filesystem::path& path)
{
    const MountPoint* mp = mount_points_.get_best_mount_point(path);
    return mp == nullptr ? -1 : mp->filesystem->stat(path.lexically_relative(mp->point));
}

std::unique_ptr<IFile> Vfs::get(const eul::filesystem::path& path, int flags)
{
    const MountPoint* mp = mount_points_.get_best_mount_point(path);
    return mp == nullptr ? nullptr : mp->filesystem->get(path.lexically_relative(mp->point), flags);
}

std::unique_ptr<IFile> Vfs::create(const eul::filesystem::path& path, int flags)
{
    const MountPoint* mp = mount_points_.get_best_mount_point(path);
    return mp == nullptr ? nullptr : mp->filesystem->create(path.lexically_relative(mp->point), flags);
}


std::vector<std::unique_ptr<IFile>> Vfs::list(const eul::filesystem::path& path)
{
    const MountPoint* mp = mount_points_.get_best_mount_point(path);
    return mp == nullptr ? std::vector<std::unique_ptr<IFile>>{} : mp->filesystem->list(path.lexically_relative(mp->point));
}

std::string_view Vfs::name() const
{
    return "VFS";
}

void Vfs::mount_fs(const eul::filesystem::path& path, IFileSystem* fs)
{
    mount_points_.mount_filesystem(path, fs);
}

IFileSystem* Vfs::get_child_fs(const eul::filesystem::path& path)
{
    const MountPoint* mp = mount_points_.get_best_mount_point(path);
    return mp == nullptr ? nullptr : mp->filesystem;
}

} // namespace fs
} // namespace msos
