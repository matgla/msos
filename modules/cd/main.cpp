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

#include <cstring>

#include <cstdio>

#include <dirent.h>
#include <unistd.h>

#include <eul/filesystem/path.hpp>

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        eul::filesystem::path target_path(argv[1]);
        // Absolute path
        if (target_path.is_absolute())
        {
            return chdir(argv[1]);
        }
        else // relative path
        {
            char cwd[255];
            getcwd(cwd, sizeof(cwd));
            std::size_t cwd_length = strlen(cwd);
            if (cwd_length >= 255)
            {
                return -1;
            }
            eul::filesystem::path path(cwd);
            path += target_path;
            path = path.lexically_normal();

            return chdir(path.c_str());
        }
    }

    return 0;
}

