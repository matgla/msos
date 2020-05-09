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

#include "msos/drivers/displays/sfml/sfml_file.hpp"

#include <eul/utils/unused.hpp>

#include "msos/libc/printf.hpp"

#include "msos/drivers/displays/sfml/sfml.hpp"

#include <iostream>

namespace msos
{
namespace fs
{

SfmlFile::SfmlFile(drivers::displays::SfmlDisplay& driver, std::string_view path)
    : driver_(driver)
    , path_(path)
{
}

ssize_t SfmlFile::read(DataType data)
{
    UNUSED1(data);
    return 0;
}

ssize_t SfmlFile::write(const ConstDataType data)
{
    driver_.write(data);
    return 0;
}

int SfmlFile::close()
{
    return 0;
}


std::string_view SfmlFile::name() const
{
    return path_;
}


std::unique_ptr<IFile> SfmlFile::clone() const
{
    return std::make_unique<SfmlFile>(*this);
}


} // namespace fs
} // namespace msos
