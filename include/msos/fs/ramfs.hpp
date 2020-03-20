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

#include "msos/fs/i_filesystem.hpp"

namespace msos
{
namespace fs
{

class RamFs : public IFileSystem
{
public:

    int mount(drivers::storage::BlockDevice& device) override;

    int umount() override;

    int create() override;

    int mkdir(const std::string_view path, int mode) override;

    int remove(const std::string_view path) override;

    int stat(const std::string_view path) override;

    IFile* get(const std::string_view path) override;
    IFile* create(const std::string_view path) override;


protected:
    static bool mounted_;
};

} // namespace fs
} // namespace msos
