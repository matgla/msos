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

#include "msos/posix/dirent_utils.hpp"

namespace msos
{
namespace posix
{
namespace dirent_utils
{

std::string_view get_next_part(std::string_view& path)
{
    // ignore slashes at start
    int dirname_start = path.find_first_not_of("/");
    if (dirname_start == std::string_view::npos)
    {
        path = {};
        return {};
    }
    path = path.substr(dirname_start, path.length());
    int next_slash = path.find("/");
    std::string_view part;
    if (next_slash != std::string_view::npos)
    {
        part = path.substr(0, next_slash);
        path = path.substr(next_slash, path.length());
    }
    else
    {
        part = path;
        path = {};
    }
    return part;
}

} // namespace dirent_utils
} // namespace posix
} // namespace msos
