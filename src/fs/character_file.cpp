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

#include "msos/fs/character_file.hpp"

#include <eul/utils/unused.hpp>

namespace msos
{
namespace fs
{

off_t CharacterFile::seek(off_t offset, int base) const
{
    UNUSED2(offset, base);
    return 1;
}

const char* CharacterFile::data() const
{
    return nullptr;
}

off_t CharacterFile::tell() const
{
    return 1;
}

ssize_t CharacterFile::size() const
{
    return 0;
}

int CharacterFile::sync()
{
    return 0;
}

} // namespace fs
} // namespace msos
