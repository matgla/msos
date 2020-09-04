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

#include "ls/printer.hpp"

#include <cstdio>

namespace msos
{
namespace ls
{

void print_directory_as_list(std::string_view path)
{
    print_directory(path, [](struct dirent* dir) {
        printf("%s\n", dir->d_name);
    });
}

void print_directory_in_line(std::string_view path)
{
    print_directory(path, [](struct dirent* dir) {
        printf("%s ", dir->d_name);
    });
    printf("\n");
}

void print_directory(std::string_view path, const Config& config)
{
    if (config.as_list())
    {
        print_directory_as_list(path);
        return;
    }

    print_directory_in_line(path);
}


} // namespace ls
} // namespace msos
