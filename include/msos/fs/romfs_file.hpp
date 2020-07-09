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

#include <romfs/fileHeader.hpp>

#include "msos/fs/file_base.hpp"

namespace msos
{
namespace fs
{

struct RomFsFile : public FileBase
{
public:
    RomFsFile(const romfs::FileHeader& fh);

    ssize_t read(DataType data) override;
    ssize_t write(const ConstDataType data) override;
    off_t seek(off_t offset, int base) const override;
    int close() override;
    int sync() override;

    off_t tell() const override;
    ssize_t size() const override;

    std::string_view name() const override;

    std::unique_ptr<IFile> clone() const override;

    const char* data() const override;

private:
    romfs::FileHeader file_;
    std::size_t position_;
};

} // namespace fs
} // namespace msos
