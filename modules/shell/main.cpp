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

#include <sys/stat.h>

#include <dirent.h>

#include "msos/kernel/process/spawn.hpp"

void ls_command(const char* path)
{
    printf("Open path: %s\n", path);
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            printf("%s ", dir->d_name);
        }
        printf("\n");
        closedir(d);
    }
}

void cd_command(std::string_view path, char* pwd)
{
    DIR *d;
    struct dirent *dir;
    char buf[100];
    int last_slash = path.find_last_of("/");
    if (last_slash != 0 && last_slash != std::string_view::npos)
    {
        std::string_view parent = path.substr(0, last_slash);

        std::memcpy(buf, parent.data(), parent.length());
        buf[parent.length() + 1] = 0;
    }

    d = opendir(buf);
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            std::string_view filename = path.substr(path.find_last_of("/"), path.length());
            if (filename == std::string_view(dir->d_name))
            {
                std::memcpy(pwd + strlen(pwd) - 1, dir->d_name, dir->d_namlen);
            }
        }
        closedir(d);
    }
}

void remove_spaces(char* s) {
    const char* d = s;
    do {
        while (isspace(*d)) {
            ++d;
        }
    } while (*s++ = *d++);
}

int main()
{
    printf("MSOS shell:\n");
    char buffer[100] = {};
    char pwd[255] = {'/', '\0'};

    while (std::string_view(buffer).find("exit") == std::string_view::npos)
    {
        write(1, "> \0", 3);
        // buffer[0] = 0;
        scanf("%s", &buffer);
        if (std::string_view(buffer).find("ls") == 0)
        {
            ls_command(pwd);
        }
        if (std::string_view(buffer).find("pwd") == 0)
        {
            printf("%s\n", pwd);
        }
        if (std::string_view(buffer).find("cd") == 0)
        {
            remove_spaces(buffer + 3);
            int current_pwd = strlen(pwd);

            cd_command(buffer + 3, pwd);
        }
    }
    printf("MSOS Shell exit\n");
}

