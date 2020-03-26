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

#include <vector>
#include <string_view>

#include "msos/fs/i_filesystem.hpp"

namespace msos
{
namespace fs
{

struct RamfsFile : public IFile
{
public:
    RamfsFile(const std::string_view name, std::vector<uint8_t>& data);

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
    std::string_view filename_;
    std::vector<uint8_t>& data_;
    std::size_t position_;
};

} // namespace fs
} // namespace msos
