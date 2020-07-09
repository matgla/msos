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

#include "msos/fs/romfs.hpp"

#include <algorithm>

#include <eul/utils/unused.hpp>

#include "msos/fs/romfs_file.hpp"
#include "msos/fs/utils.hpp"

namespace msos
{
namespace fs
{

RomFs::RomFs(const uint8_t* memory)
    : disk_(memory)
{

}

int RomFs::mount(drivers::storage::BlockDevice& device)
{
    UNUSED1(device);
    return 1;
}

int RomFs::umount()
{
    return 1;
}

int RomFs::stat(const eul::filesystem::path& path)
{
    UNUSED1(path);
    return 1;
}

std::unique_ptr<IFile> RomFs::get(const eul::filesystem::path& path, int flags)
{
    UNUSED1(flags);

    eul::filesystem::path path_in_fs("/");
    if (path.is_absolute())
    {
        path_in_fs = path;
    }
    else
    {
        path_in_fs += path;
    }
    if (path.native().empty())
    {
        path_in_fs = "/";
    }

    auto dir = disk_.get_directory(path_in_fs.c_str());
    if (dir)
    {
        return std::make_unique<RomFsFile>(dir->get_file_header());
    }


    auto filename = path_in_fs.filename();
    dir = disk_.get_directory(path_in_fs.parent_path().native());
    if (dir)
    {
        auto file = dir->get_file(filename);
        if (file)
        {
            return std::make_unique<RomFsFile>(*file);
        }
    }

    return nullptr;
}

std::vector<std::unique_ptr<IFile>> RomFs::list(const eul::filesystem::path& path)
{
    eul::filesystem::path path_in_fs("/");
    if (path.is_absolute())
    {
        path_in_fs = path;
    }
    else
    {
        path_in_fs += path;
    }

    std::vector<std::unique_ptr<IFile>> files;
    auto dir = disk_.get_directory(path_in_fs.native());
    if (!dir)
    {
        return {};
    }

    for (auto& file : *dir)
    {
        insert_with_order(files, std::make_unique<RomFsFile>(file));
    }

    return files;
}

std::string_view RomFs::name() const
{
    return "RomFS";
}


} // namespace fs
} // namespace msos
