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

#include <string_view>
#include <string>

#include "msos/fs/i_filesystem.hpp"
#include "msos/fs/mount_points.hpp"

namespace msos
{
namespace fs
{

class Vfs : public IFileSystem
{
public:
    static Vfs& instance();
    int mount(drivers::storage::BlockDevice& device) override;

    int umount() override;

    int create() override;

    int mkdir(const eul::filesystem::path& path, int mode) override;

    int remove(const eul::filesystem::path& path) override;

    int stat(const eul::filesystem::path& path) override;

    std::unique_ptr<IFile> get(const eul::filesystem::path& path) override;
    std::unique_ptr<IFile> create(const eul::filesystem::path& path) override;

    std::vector<std::unique_ptr<IFile>> list(const eul::filesystem::path& path) override;

    void mount_fs(const eul::filesystem::path& path, IFileSystem* fs);
    std::string_view name() const override;
    IFileSystem* get_child_fs(const eul::filesystem::path& path);
private:

    MountPoints mount_points_;
};

} // namespace fs
} // namespace msos
