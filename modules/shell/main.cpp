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

#include <eul/filesystem/path.hpp>

#include "msos/kernel/process/spawn.hpp"

void ls_command(const char* path)
{
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

void remove_whitespace_from_start(std::string_view& path)
{
    int first_char = path.find_first_not_of(" ");
    if (first_char != 0 && first_char != std::string_view::npos)
    {
        path = path.substr(first_char, path.length());
    }
}

bool is_absolute_path(const std::string_view& path)
{
    if (path.empty())
    {
        return false;
    }
    return path[0] == '/';
}

void cd_command(std::string_view path, char* pwd)
{
    DIR *d;
    struct dirent *dir;
    remove_whitespace_from_start(path);

    if (is_absolute_path(path))
    {
        DIR *dir = opendir(path.data());
        if (dir != nullptr)
        {
            std::memcpy(pwd, path.data(), path.length());
            closedir(dir);
            return;
        }
    }
    else
    {
        char absolute_path[100];
        int pwd_length = strlen(pwd);
        if (pwd_length + path.length() + 2 >= 100)
        {
            return;
        }
        std::memcpy(absolute_path, pwd, pwd_length);
        if (pwd_length != 1)
        {
            absolute_path[pwd_length] = '/';
            std::memcpy(absolute_path + pwd_length + 1, path.data(), path.length());
            absolute_path[path.length() + pwd_length + 1] = 0;
        }
        else
        {
            std::memcpy(absolute_path + pwd_length, path.data(), path.length());
            absolute_path[path.length() + pwd_length] = 0;
        }
        DIR *dir = opendir(absolute_path);
        if (dir != nullptr)
        {
            closedir(dir);
            std::memcpy(pwd, absolute_path, strlen(absolute_path));
            return;
        }
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

std::string_view get_next_argument(std::string_view& arglist)
{
    // ignore slashes at start
    int dirname_start = arglist.find_first_not_of(" ");
    if (dirname_start == std::string_view::npos)
    {
        arglist = {};
        return {};
    }
    arglist = arglist.substr(dirname_start, arglist.length());
    int next_slash = arglist.find(" ");
    std::string_view part;
    if (next_slash != std::string_view::npos)
    {
        part = arglist.substr(0, next_slash);
        arglist = arglist.substr(next_slash, arglist.length());
    }
    else
    {
        part = arglist;
        arglist = {};
    }
    return part;
}

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
    printf("MSOS shell:\n");
    char buffer[100] = {};
    char pwd[100] = {'/', '\0'};

    while (std::string_view(buffer).find("exit") == std::string_view::npos)
    {
        write(1, "> \0", 3);
        // buffer[0] = 0;
        scanf("%s", &buffer);
        if (std::string_view(buffer).find("ls") == 0)
        {
            ls_command(pwd);
        }
        else if (std::string_view(buffer).find("pwd") == 0)
        {
            printf("%s\n", pwd);
        }
        else if (std::string_view(buffer).find("cd") == 0)
        {
            remove_newlines(buffer, strlen(buffer));
            std::string_view arglist(buffer);
            get_next_argument(arglist);
            eul::filesystem::path path(get_next_argument(arglist)); // first argument is command name
            cd_command(path.lexically_normal().native(), pwd);
        }
        else
        {
            remove_newlines(buffer, strlen(buffer));
            std::string_view path(buffer);
            if (is_absolute_path(buffer))
            {
                printf("executing %s\n", path);
                exec(buffer, NULL, NULL, NULL);
            }
            else
            {
                char absolute_path[100];
                int pwd_length = strlen(pwd);
                if (pwd_length + path.length() + 2 >= 100)
                {
                    continue;
                }
                std::memcpy(absolute_path, pwd, pwd_length);
                if (pwd_length != 1)
                {
                    printf("Pwd: %s\n", pwd);
                    absolute_path[pwd_length] = '/';
                    std::memcpy(absolute_path + pwd_length + 1, path.data(), path.length());
                    absolute_path[path.length() + pwd_length + 1] = 0;
                }
                else
                {
                    printf("Pwd not: %s\n", pwd);
                    std::memcpy(absolute_path + pwd_length, path.data(), path.length());
                    absolute_path[path.length() + pwd_length] = 0;
                }
                printf("executing %s\n", absolute_path);
                exec(absolute_path, NULL, NULL, NULL);

            }

        }
    }
    printf("MSOS Shell exit\n");
}

