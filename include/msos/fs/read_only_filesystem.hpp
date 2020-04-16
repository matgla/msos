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

#include "msos/fs/i_filesystem.hpp"

namespace msos
{
namespace fs
{

class ReadOnlyFileSystem : public IFileSystem
{
public:
    int create() override;
    int mkdir(const eul::filesystem::path& path, int mode) override;
    int remove(const eul::filesystem::path& path) override;
    std::unique_ptr<IFile> create(const eul::filesystem::path& path) override;
};

} // namespace fs
} // namespace msos
