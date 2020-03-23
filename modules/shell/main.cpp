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

#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <string_view>

int main()
{
    printf("MSOS shell:\n");
    write(1, "> \0", 3);
    char buffer[100] = {};
    while (std::string_view(buffer).find("exit") == std::string_view::npos)
    {
        write(1, "> \0", 3);
        // buffer[0] = 0;
        scanf("%s", &buffer);
        if (std::string_view(buffer).find("ls") != std::string_view::npos)
        {
            printf("There will be ls in future\n");
        }
    }
    printf("MSOS Shell exit\n");
}

