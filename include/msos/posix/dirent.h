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

#include <cstdint>

extern "C"
{

typedef struct dirent {
    uint32_t d_ino;
    uint32_t d_off;
    uint16_t d_reclen;
    uint32_t d_namlen;
    int d_type;
    char d_name[128];
} dirent;

struct DIRImpl;

typedef struct DIR {
    struct dirent ent;
    struct DIRImpl *impl;
} DIR;

DIR* opendir(const char* dirname);
dirent* readdir(DIR *dirp);
int closedir(DIR *dirp);

}
