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

#include "msos/fs/usart_file.hpp"

#include <eul/utils/unused.hpp>

#include "msos/usart_printer.hpp"

namespace msos
{
namespace fs
{

static UsartWriter writer;

ssize_t UsartFile::read(DataType data)
{
    UNUSED1(data);
    return 0;
}

ssize_t UsartFile::write(const ConstDataType data)
{
    writer << std::string_view(data.data(), data.size());
    return data.size();
}

off_t UsartFile::seek(off_t offset, int base) const
{
    UNUSED2(offset, base);
    return 0;
}

int UsartFile::close()
{
    return 0;
}

int UsartFile::sync()
{
    return 0;
}

off_t UsartFile::tell() const
{
    return 0;
}

ssize_t UsartFile::size() const
{
    return 0;
}

std::string_view UsartFile::name() const
{
    return "USART";
}

std::unique_ptr<IFile> UsartFile::clone() const
{
    return std::make_unique<UsartFile>();
}

const char* UsartFile::data() const
{
    return nullptr;
}

} // namespace fs
} // namespace msos
