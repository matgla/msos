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

#include "msos/fs/ramfs.hpp"
#include "msos/fs/ramfs_file.hpp"

#include <eul/utils/unused.hpp>

namespace msos
{
namespace fs
{

int RamFs::mount(drivers::storage::BlockDevice& device)
{
    UNUSED1(device);
    return 1;
}

int RamFs::umount()
{
    return 1;
}

int RamFs::create()
{
    return 1;
}

int RamFs::mkdir(std::string_view path, int mode)
{
    UNUSED2(path, mode);
    return 1;
}

int RamFs::remove(std::string_view path)
{
    UNUSED1(path);
    return 1;
}

int RamFs::stat(std::string_view path)
{
    UNUSED1(path);
    return 1;
}

std::unique_ptr<IFile> RamFs::get(std::string_view path)
{
    for (auto& file : files_)
    {
        if (file.filename == path)
        {
            return std::make_unique<RamfsFile>(path, file.data);
        }
    }
    return nullptr;
}

std::unique_ptr<IFile> RamFs::create(std::string_view path)
{
    files_.push_back(RamFsData{path});
    return std::make_unique<RamfsFile>(path, files_.back().data);
}

std::vector<std::unique_ptr<IFile>> RamFs::list(std::string_view path)
{
    return {};
}

} // namespace fs
} // namespace msos
