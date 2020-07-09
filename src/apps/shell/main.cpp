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
#include <fcntl.h>
#include <unistd.h>
#include <string_view>

#include <sys/stat.h>

#include <dirent.h>

#include <eul/filesystem/path.hpp>
#include "msos/apps/app_registry.hpp"

#include "msos/kernel/process/spawn.hpp"

void ls_command(const char* path)
{
    static_cast<void>(path);
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
bool is_absolute_path(const std::string_view& path)
{
    if (path.empty())
    {
        return false;
    }
    return path[0] == '/';
}

void cd_command(const eul::filesystem::path& path, char* pwd)
{
    if (path.is_absolute())
    {
        DIR *d = opendir(path.c_str());
        if (d != nullptr)
        {
            std::memcpy(pwd, path.c_str(), path.native().length());
            pwd[path.native().length()] = 0;
            closedir(d);
            return;
        }
    }
    else
    {
        eul::filesystem::path absolute_path(pwd);
        absolute_path += path;
        auto ab = absolute_path.lexically_normal(); // first argument is command name
        DIR *d = opendir(ab.c_str());
        if (d != nullptr)
        {
            closedir(d);

            std::memcpy(pwd, ab.c_str(), ab.native().length());
            pwd[ab.native().length()] = 0;
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
    } while ((*s++ = *d++));
}

std::string_view get_next_argument(std::string_view& arglist)
{
    // ignore slashes at start
    std::size_t dirname_start = arglist.find_first_not_of(" ");
    if (dirname_start == std::string_view::npos)
    {
        arglist = {};
        return {};
    }
    arglist = arglist.substr(dirname_start, arglist.length());
    std::size_t next_slash = arglist.find(" ");
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

int app_start()
{
    printf("MSOS shell:\n");

    for (const auto& app : msos::apps::AppRegistry::get_instance().get_apps())
    {
        if (app.autostart)
        {
            std::string path = "/bin/";
            path += app.name;
            exec(path.c_str(), NULL, NULL, 0);
        }
    }

    char buffer[100] = {};
    char pwd[100] = {'/', '\0'};

    while (std::string_view(buffer).find("exit") == std::string_view::npos)
    {
        write(1, "> \0", 2);
        fgets(buffer, sizeof(buffer), stdin);
        // // scanf("%s", &buffer); // <- replace to fgets
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
            cd_command(path.lexically_normal(), pwd);
        }
        else if (std::string_view(buffer).find("exit") == 0)
        {
            break;
        }
        else
        {
            remove_newlines(buffer, strlen(buffer));
            eul::filesystem::path path(buffer); // first argument is command name

            if (is_absolute_path(buffer))
            {
                printf("executing %s\n", path.c_str());
                exec(buffer, NULL, NULL, 0);
            }
            else
            {
                char absolute_path[100];
                std::size_t pwd_length = strlen(pwd);
                if (pwd_length + path.native().length() + 2 >= 100)
                {
                    continue;
                }
                std::memcpy(absolute_path, pwd, pwd_length);
                if (pwd_length != 1)
                {
                    printf("Pwd: %s\n", pwd);
                    absolute_path[pwd_length] = '/';
                    std::memcpy(absolute_path + pwd_length + 1, path.c_str(), path.native().length());
                    absolute_path[path.native().length() + pwd_length + 1] = 0;
                }
                else
                {
                    printf("Pwd not: %s\n", pwd);
                    std::memcpy(absolute_path + pwd_length, path.c_str(), path.native().length());
                    absolute_path[path.native().length() + pwd_length] = 0;
                }
                printf("executing %s\n", absolute_path);
                exec(absolute_path, NULL, NULL, 0);
            }
        }
    }
    printf("MSOS Shell exit\n");
    return 0;
}

REGISTER_APP(msos_shell, &app_start);
