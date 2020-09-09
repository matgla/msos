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

#include <dirent.h>
#include <unistd.h>

#include "ls/config.hpp"
#include "ls/printer.hpp"



int main(int argc, char *argv[])
{
    int c;

    msos::ls::Config config;

    while ((c = getopt(argc, argv, "l")) != -1)
    {
        switch (c)
        {
            case 'l':
            {
                config.set_as_list();
            } break;
            case '?':
            {
                printf("Write `ls -h` to get more informations.\n");
            return -1;
            } break;
        }
    }

    /* directory not specified */
    if (optind >= argc)
    {
        char cwd[255] = {0};
        getcwd(cwd, sizeof(cwd));

        msos::ls::print_directory(cwd, config);
        return 0;
    }

    bool more_than_one_directory_to_list = argc - optind > 1 ? true : false;
    for (int index = optind; index < argc; index++)
    {
        if (more_than_one_directory_to_list)
        {
            printf("%s:\n", argv[index]);
        }
        msos::ls::print_directory(argv[index], config);
        if (more_than_one_directory_to_list)
        {
            printf("\n");
        }
    }

    return(0);
}

