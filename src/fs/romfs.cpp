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

#include "msos/fs/romfs_file.hpp"

#include <eul/utils/unused.hpp>

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

int RomFs::create()
{
    return 1;
}

int RomFs::mkdir(std::string_view path, int mode)
{
    UNUSED2(path, mode);
    return -1;
}

int RomFs::remove(std::string_view path)
{
    UNUSED1(path);
    return -1;
}

int RomFs::stat(std::string_view path)
{
    UNUSED1(path);
    return 1;
}

std::unique_ptr<IFile> RomFs::get(std::string_view path)
{
    printf("Searching directory: %s\n", path.data());
    auto dir = disk_.get_directory(path);
    if (dir)
    {
        printf("Found %s\n", path.data());
        return nullptr;
    }
    std::size_t last_slash = path.rfind("/");
    printf("in %s last / is at %d\n", path.data(), last_slash);
    std::string_view filename = path.substr(last_slash + 1, path.size());
    printf("Found directory %d\n", filename.size());

    path.remove_suffix(path.size() - last_slash);
    dir = disk_.get_directory(path);
    if (dir)
    {
        printf("Found directory %s\n", path.data());
        printf("Searching file %s\n", filename.data());
        auto file = dir->get_file(filename);
        if (file)
        {
            printf("Got file: %p\n", file->data());
            return std::make_unique<RomFsFile>(*file);
        }
    }

    dir = disk_.get_directory("/");
    if (dir)
    {
        printf("Got root directory, files inside: \n");
        for (const auto& file : *dir)
        {
            printf("%s\n", file.get_name().data());
        }
    }

    return nullptr;
}

std::unique_ptr<IFile> RomFs::create(std::string_view path)
{
    UNUSED1(path);
    return nullptr;
}

} // namespace fs
} // namespace msos
