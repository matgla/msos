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

#include <string_view>

#include <board.hpp>

#include "msos/drivers/character/usart/usart_file.hpp"
#include "msos/usart_printer.hpp"

namespace msos
{
namespace fs
{

UsartFile::UsartFile(drivers::character::UsartDriver& driver, std::string_view path)
    : driver_(driver)
    , path_(path)
{
}

ssize_t UsartFile::read(DataType data)
{
    ssize_t length = 0;
    auto& buffer = driver_.buffer();
    const ssize_t length_to_read = static_cast<std::size_t>(data.size()) < buffer.size() ? data.size() : buffer.size();
    while (length < length_to_read)
    {
        data[length] = buffer.pop();
        ++length;
    }
    return length;
}
ssize_t UsartFile::write(const ConstDataType data)
{
    driver_.get().write(gsl::span<const uint8_t>(
        reinterpret_cast<const uint8_t*>(data.data()), data.size()));
    return data.size();
}

int UsartFile::close()
{
    return 0;
}

std::string_view UsartFile::name() const
{
    return path_;
}


std::unique_ptr<IFile> UsartFile::clone() const
{
    return std::make_unique<UsartFile>(*this);
}

void UsartFile::stat(struct stat& s) const
{
    s.st_mode = 0;
    s.st_mode |= S_IFCHR;
}

} // namespace fs
} // namespace msos
