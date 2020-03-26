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
#include <cstdio>
#include <string_view>

#include "msos/usart_printer.hpp"

int __vfscanf_(int fd, const char* format, va_list argptr,
                    unsigned int mode_flags)
{
    if (std::string_view(format) == "%s")
    {
        char* buffer = va_arg(argptr, char*);
        return read(fd, buffer, 0);
    }
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
    UsartWriter writer;
    writer << format;
    return 0;
}


// 2}
