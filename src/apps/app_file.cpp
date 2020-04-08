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

#include "msos/apps/app_file.hpp"

#include <eul/utils/unused.hpp>

namespace msos
{
namespace apps
{


AppFile::AppFile(const AppEntry entry)
    : entry_(entry)
{

}

AppFile::AppFile()
{

}


ssize_t AppFile::read(DataType data)
{
    UNUSED1(data);
    return 0;
}

ssize_t AppFile::write(const ConstDataType data)
{
    UNUSED1(data);
    return 0;
}

off_t AppFile::seek(off_t offset, int base) const
{
    UNUSED2(offset, base);
    return 0;
}

int AppFile::close()
{
    return 0;
}

int AppFile::sync()
{
    return 0;
}

off_t AppFile::tell() const
{
    return 0;
}

ssize_t AppFile::size() const
{
    return 0;
}

std::string_view AppFile::name() const
{
    return entry_.name;
}

std::unique_ptr<fs::IFile> AppFile::clone() const
{
    return nullptr;
}

const char* AppFile::data() const
{
    return reinterpret_cast<const char*>(entry_.address);
}

} // namespace apps
} // namespace msos
