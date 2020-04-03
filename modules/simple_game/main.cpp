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

void remove_newlines(char* str, size_t len)
{
    int number_of_newlines = 0;
    for (size_t i = 0; i < len; i++)
    {
        if (str[i] == '\n' || str[i] == '\r')
        {
            ++number_of_newlines;
        }
        if (i + number_of_newlines >= len)
        {
            str[i] = 0;
        }
        else
        {
            str[i] = str[i + number_of_newlines];
        }

    }
}

int main()
{
    char name[20];
    printf("Hello player, please write your name: ");
    scanf("%s", name);
    remove_newlines(name, 20);
    printf("Hello %s, but game is not finished yet:(\n", name);
}

