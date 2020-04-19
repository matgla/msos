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

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string_view>
#include <type_traits>
#include <cstdint>

#include <unistd.h>

#include <eul/utils/unused.hpp>

#include "msos/usart_printer.hpp"

static UsartWriter writer_b_;

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

char* _fgets(char* buffer, int n, FILE* fp)
{
    int readed = -1;
    int fd = fp->_file;
    char c = 0;
    while (c != '\n')
    {
        if (!read(fd, &c, 1))
        {
            continue;
        }
        if (c == '\b')
        {
            if (readed >= 0)
            {
                --readed;
                ++n;
            }
            continue;
        }
        buffer[++readed] = c;
        --n;
        if (n == 1)
        {
            break;
        }
    }
    buffer[readed] = 0;

    return nullptr;

}

int __vfscanf_(int fd, const char* format, va_list argptr,
                    unsigned int mode_flags)
{
    UNUSED1(mode_flags);
    int readed = -1;
    if (std::string_view(format) == "%s")
    {
        char* buffer = va_arg(argptr, char*);
        char c = 0;
        while (c != '\n')
        {
            if (!read(fd, &c, 1))
            {
                continue;
            }
            if (c == '\b')
            {
                if (readed >= 0)
                {
                    --readed;
                }
                continue;
            }
            buffer[++readed] = c;
        }
        buffer[readed] = 0;
        int i =0;
        while (true)
        {
            if (buffer[i] == 0)
            {
                break;
            }
            ++i;
        }
    }

    return readed;
}

int _scanf(const char* format, ...)
{
    va_list arg;
    va_start (arg, format);
    int done = __vfscanf_(0, format, arg, 0);
    va_end (arg);
    return done;
}

int _printf(const char* format, ...)
{
    va_list arg;
    va_start (arg, format);
    int done = __vfprintf_(writer_b_, 1, format, arg, 0);
    va_end (arg);
    return done;
}

} // extern "C"
