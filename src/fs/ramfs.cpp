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

#include "msos/fs/ramfs_file.hpp"

#include "msos/posix/dirent_utils.hpp"
#include "msos/fs/mount_points.hpp"

#include <eul/utils/unused.hpp>

#include "msos/usart_printer.hpp"

namespace msos
{
namespace fs
{

static UsartWriter writer;

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
    std::size_t last_slash = path.find_last_of("/");
    std::size_t dirname_start = path.find_first_not_of("/");
    if (dirname_start == std::string_view::npos)
    {
        return -1;
    }
    path = path.substr(dirname_start, path.length());
    if (last_slash == std::string_view::npos || last_slash == 0)
    {
        files_.push_back(RamFsData{path});
        return 0;
    }
    else
    {
        std::string_view parent_path = path.substr(0, last_slash);
        auto parent = std::find_if(files_.begin(), files_.end(), [parent_path](const auto& file) {
            return file.filename == parent_path;
        });
        if (parent != files_.end())
        {
            files_.push_back(RamFsData{path});
            return 0;
        }

    }
    return -1;
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
    std::size_t dirname_start = path.find_first_not_of("/");
    if (dirname_start != std::string_view::npos)
    {
        path = path.substr(dirname_start, path.length());
    }
    else if (!path.empty())
    {
        return std::make_unique<RamfsFile>("/");
    }

    int best_match_value = INT_MAX;
    std::string_view best_match;
    for (auto& file : files_)
    {
        int match = std::abs(file.filename.compare(path));
        if (match == 0 && mount_points.get_mount_point(file.filename) == nullptr)
        {
            return std::make_unique<RamfsFile>(path, file.data);
        }
        else if (match < best_match_value)
        {
            best_match_value = match;
            best_match = file.filename;
        }
    }

    // try to find mounted point

    const auto* mp = msos::fs::mount_points.get_mount_point(best_match);
    if (mp)
    {
        std::string_view child_path = path.substr(best_match.length(), path.length());
        if (child_path.empty())
        {
            child_path = "/";
        }
        auto file = mp->filesystem->get(child_path);
        return std::move(file);
    }

    return nullptr;
}

std::unique_ptr<IFile> RamFs::create(std::string_view path)
{
    std::size_t dirname_start = path.find_first_not_of("/");
    if (dirname_start == std::string_view::npos)
    {
        return nullptr;
    }
    path = path.substr(dirname_start, path.length());
    files_.push_back(RamFsData{path});
    return std::make_unique<RamfsFile>(path, files_.back().data);
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

std::vector<std::unique_ptr<IFile>> RamFs::list(std::string_view path)
{
    // writer << "List: " << endl;
    std::vector<std::unique_ptr<IFile>> files;

    std::size_t dirname_start = path.find_first_not_of("/");
    if (dirname_start != std::string_view::npos)
    {
        path = path.substr(dirname_start, path.length());
    }
    else if (!path.empty())
    {
        path = {};
    }

    // writer << "Path: " << path;
    if (path.empty())
    {
        // writer << "Path is empty: " << path;

        for (auto& file : files_)
        {
            // writer << file.filename << endl;
            if (file.filename.find("/") == std::string_view::npos)
            {
                // writer << "Adding file: " << file.filename << endl;
                files.push_back(std::make_unique<RamfsFile>(file.filename, file.data));
            }
        }
    }
    else
    {
        std::size_t last_slash = path.find_last_of("/");
        std::string_view parent_path = path;

        if (last_slash != std::string_view::npos)
        {
            parent_path = path.substr(0, last_slash);
        }
        for (auto& file : files_)
        {
            std::string_view file_parent_path = get_parent_path(file.filename);
            if (file_parent_path == parent_path)
            {
                files.push_back(std::make_unique<RamfsFile>(file.filename, file.data));
            }
        }

    }

    std::sort(files.begin(), files.end(), [](const auto& lhs, const auto& rhs) {
        return std::lexicographical_compare(lhs->name().begin(), lhs->name().end(), rhs->name().begin(), rhs->name().end());
    });
    return files;
}

} // namespace fs
} // namespace msos
