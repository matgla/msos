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

#include <romfs/romfs.hpp>

#include "msos/fs/i_filesystem.hpp"

namespace msos
{
namespace fs
{

class RomFs : public IFileSystem
{
public:
    RomFs(const uint8_t* memory);

    int mount(drivers::storage::BlockDevice& device) override;

    int umount() override;

    int create() override;

    int mkdir(std::string_view path, int mode) override;

    int remove(std::string_view path) override;

    int stat(std::string_view path) override;

    std::unique_ptr<IFile> get(std::string_view path) override;
    std::unique_ptr<IFile> create(std::string_view path) override;

    std::vector<std::unique_ptr<IFile>> list(std::string_view path) override;
    std::string_view name() const override;

protected:
    static bool mounted_;
    romfs::RomFsDisk disk_;

};

} // namespace fs
} // namespace msos
