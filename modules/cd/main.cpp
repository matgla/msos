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

#include <dirent.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        // Absolute path
        if (argv[1][0] == '/')
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
            if (cwd[cwd_length - 1] != '/')
            {
                cwd[cwd_length] = '/';
                cwd[++cwd_length] = 0;

            }
            std::size_t rest_space = sizeof(cwd) - cwd_length;
            std::size_t suffix_space = strlen(argv[1]);
            std::size_t to_copy = rest_space < suffix_space ? rest_space : suffix_space;
            std::memcpy(cwd + cwd_length, argv[1], to_copy);
            cwd_length += to_copy;
            cwd[cwd_length] = 0;
            return chdir(cwd);
        }
    }

    return 0;
}

