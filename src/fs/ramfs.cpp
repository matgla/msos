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

#include <algorithm>
#include <climits>

#include <eul/utils/unused.hpp>

#include "msos/fs/ramfs_file.hpp"
#include "msos/fs/utils.hpp"

#include "msos/usart_printer.hpp"

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

int RamFs::mkdir(const eul::filesystem::path& path, int mode)
{
    UNUSED1(mode);
    auto it = std::find_if(files_.begin(), files_.end(), [path](const auto& file) {
        return file.filename() == path.native();
    });
    if (it != files_.end())
    {
        return -1;
    }
    files_.push_back(RamFsData{path});
    return 0;
}

int RamFs::remove(const eul::filesystem::path& path)
{
    UNUSED1(path);
    return 1;
}

int RamFs::stat(const eul::filesystem::path& path)
{
    UNUSED1(path);
    return 1;
}

std::unique_ptr<IFile> RamFs::get(const eul::filesystem::path& path)
{
    if (path.native() == "/" || path.native().empty())
    {
        return std::make_unique<RamfsFile>("/");
    }
    for (auto& file : files_)
    {
        if (file.filename() == path.native())
        {
            return std::make_unique<RamfsFile>(path.native(), file.data());
        }
    }

    return nullptr;
}

std::unique_ptr<IFile> RamFs::create(const eul::filesystem::path& path)
{
    files_.push_back(RamFsData{path});
    return std::make_unique<RamfsFile>(files_.back().filename(), files_.back().data());
}

std::string_view get_parent_path(std::string_view path)
{
    std::size_t last_slash = path.find_last_of("/");
    std::string_view parent_path;

    if (last_slash != std::string_view::npos)
    {
        parent_path = path.substr(0, last_slash);
    }

    return parent_path;
}

std::vector<std::unique_ptr<IFile>> RamFs::list(const eul::filesystem::path& path)
{
    std::vector<std::unique_ptr<IFile>> files;

    if (path.native().empty() || path.native() == "/")
    {
        for (auto& file : files_)
        {
            if (file.filename().find("/") == std::string_view::npos)
            {
                insert_with_order(files, std::make_unique<RamfsFile>(file.filename(), file.data()));
            }
        }
    }
    else
    {
        for (auto& file : files_)
        {
            std::string_view file_parent_path = get_parent_path(file.filename());
            if (file_parent_path == path.parent_path().native())
            {
                insert_with_order(files, std::make_unique<RamfsFile>(file.filename(), file.data()));
            }
        }

    }

    return files;
}

std::string_view RamFs::name() const
{
    return "RamFs";
}


} // namespace fs
} // namespace msos
