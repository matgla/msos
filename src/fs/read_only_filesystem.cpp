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

#include "msos/fs/read_only_filesystem.hpp"

#include <eul/utils/unused.hpp>

namespace msos
{
namespace fs
{

int ReadOnlyFileSystem::create(int flags)
{
    UNUSED1(flags);
    return -1;
}

int ReadOnlyFileSystem::mkdir(const eul::filesystem::path& path, int mode)
{
    UNUSED2(path, mode);
    return -1;
}

int ReadOnlyFileSystem::remove(const eul::filesystem::path& path)
{
    UNUSED1(path);
    return -1;
}

std::unique_ptr<IFile> ReadOnlyFileSystem::create(const eul::filesystem::path& path, int flags)
{
    UNUSED2(path, flags);
    return nullptr;
}

} // namespace fs
} // namespace msos
