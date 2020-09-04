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
