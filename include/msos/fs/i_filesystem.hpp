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
#include <vector>

#include <eul/filesystem/path.hpp>

#include "msos/drivers/storage/block_device.hpp"

#include "msos/fs/i_file.hpp"

namespace msos
{
namespace fs
{

class IFileSystem
{
public:
    virtual ~IFileSystem() = default;

    virtual int mount(drivers::storage::BlockDevice& device) = 0;
    virtual int umount() = 0;

    virtual int create() = 0;
    virtual int mkdir(const eul::filesystem::path& path, int mode) = 0;

    virtual int remove(const eul::filesystem::path& path) = 0;
    virtual int stat(const eul::filesystem::path& path) = 0;
    virtual std::string_view name() const = 0;

    virtual std::vector<std::unique_ptr<IFile>> list(const eul::filesystem::path& path) = 0;

    virtual std::unique_ptr<IFile> get(const eul::filesystem::path& path) = 0;
    virtual std::unique_ptr<IFile> create(const eul::filesystem::path& path) = 0;
};

} // namespace fs
} // namespace msos
