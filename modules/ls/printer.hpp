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

#include <dirent.h>
#include <unistd.h>

#include "ls/config.hpp"

namespace msos
{
namespace ls
{

template <typename Printer>
void print_directory(std::string_view path, const Printer& printer)
{
    DIR *d;
    struct dirent *dir;

    d = opendir(path.data());
    if (d)
    {
        while ((dir = readdir(d)) != nullptr)
        {
            printer(dir);
        }
        closedir(d);
    }
}

void print_directory(std::string_view path, const Config& config);

} // namespace ls
} // namespace msos
