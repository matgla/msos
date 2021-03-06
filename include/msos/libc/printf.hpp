// This file is part of MSOS project. This is simple OS for embedded development devices.
// Copyright (C) 2019 Mateusz Stadnik
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

#include <cstring>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string_view>
#include <type_traits>
#include <cstdint>

#include <unistd.h>

#include <eul/utils/unused.hpp>


template <typename T>
int __vfprintf_(T& writer, int fd, const char* format, va_list argptr,
                    unsigned int mode_flags)
{
    UNUSED2(fd, mode_flags);
    std::string_view data(format);
    std::size_t position = 0, previous_position = 0, size = 0;
    //  %[flags][width][.precision][length]specifier
    do
    {
        position = data.find("%");
        if (position == std::string_view::npos)
        {
            writer << std::string_view(data.data() + previous_position, data.length());
            return static_cast<int>(size + data.length());
        }
        if (position > 0)
        {
            writer << data.substr(previous_position, position);
            size += data.substr(previous_position, position).length();
        }
        if (position + 1 >= data.length())
        {
            return static_cast<int>(size + data.length());
        }
        ++position;
        char specifier = data[position];
        switch (specifier)
        {
            case 'c': {
                char arg = static_cast<char>(va_arg(argptr, int));
                writer << arg;
            } break;
            case 's': {
                const char* arg = va_arg(argptr, const char*);
                writer << arg;
            } break;
            case 'd': {
                int arg = va_arg(argptr, int);
                writer << arg;
            }
        }
        if (position + 1 >= data.length())
        {
            return static_cast<int>(size + data.length());
        }
        ++position;

        data = data.substr(position, data.length());
    } while (position != std::string_view::npos);
    return static_cast<int>(size);
}

extern "C"
{
    int _printf(const char* format, ...);
    int _scanf(const char* format, ...);
    char* _fgets(char* buffer, int n, FILE* fp);
    int _fputs(const char* buffer, FILE* fp);
    int fileno(FILE *stream);

    #define printf _printf
    #define scanf _scanf
    #define fgets _fgets
    #define fputs _fputs
}
