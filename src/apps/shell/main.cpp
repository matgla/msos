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
#include "msos/libc/printf.hpp"

#include "msos/kernel/process/spawn.hpp"

#include "shell/command_parser.hpp"

bool is_absolute_path(const std::string_view& path)
{
    if (path.empty())
    {
        return false;
    }
    return path[0] == '/';
}

namespace msos
{
namespace shell
{

void read_configuration_file()
{
    FILE* fp = fopen("/config/shell.cfg", "r");
    if (fp == nullptr)
    {
        fp = fopen("/rom/config/shell.cfg", "r");
    }

    if (fp == nullptr)
    {
        return;
    }

    char buffer[255];
    fgets(buffer, sizeof(buffer), fp);
    printf("%s\n", buffer);
}

int app_start()
{
    printf("Start sheel\n");
    read_configuration_file();

    char buffer[100] = {};
    char* argv[10];

    char* path_env = std::getenv("path");
    if (path_env)
    {
        printf("Path found: %s\n", path_env);
    }
    else
    {
        printf("Path not found!\n");
    }
    while (std::string_view(buffer).find("exit") == std::string_view::npos)
    {
        write(1, "> \0", 2);
        std::memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        std::string_view request(buffer);

        if(std::all_of(request.begin(), request.end(), isspace))
        {
            continue;
        }
        if (request.find("exit") != std::string_view::npos)
        {
            return 0;
        }

        /* find binary in path environment variable */
        char cwd[255];
        getcwd(cwd, sizeof(cwd));
        eul::filesystem::path path_to_cwd(cwd);

        int argc = 1;

        // preprocess request
        request = request.substr(request.find_first_not_of(' '));

        if (is_absolute_path(request))
        {
            argv[0] = const_cast<char*>(request.data());
            exec(request.data(), argc, argv, NULL, 0);
            continue;
        }

        CommandParser parser(buffer, strlen(buffer));
        path_to_cwd += parser.get_command();

        while (!parser.empty())
        {
            argv[argc] = const_cast<char*>(parser.get_next_argument().data());
            ++argc;
        }

        argv[0] = const_cast<char*>(path_to_cwd.c_str());

        // check if file exists in current directory

        FILE* file = fopen(path_to_cwd.c_str(), "r");

        bool command_found = false;
        if (file)
        {
            fclose(file);
            command_found = true;
            exec(path_to_cwd.c_str(), argc, argv, NULL, 0);
            continue;
        }

        std::string_view path(path_env);

        msos::shell::Splitter splitter(path_env, strlen(path_env), ';', false);

        while (!splitter.empty())
        {
            DIR *d;
            struct dirent *dir;

            std::string_view next_dir = splitter.get_next_part();
            d = opendir(next_dir.data());
            while ((dir = readdir(d)) != nullptr)
            {
                if (std::string_view(dir->d_name) == parser.get_command())
                {
                    eul::filesystem::path path_to_binary(next_dir);
                    path_to_binary += parser.get_command();
                    argv[0] = const_cast<char*>(path_to_binary.c_str());
                    for (int i = 0; i < argc; ++i)
                    {
                        printf("ARGV[%d]: ", i);
                        for (size_t y = 0; y <= strlen(argv[i]); ++y)
                        {
                            printf("%d[%c], ", argv[i][y], argv[i][y]);
                        }
                        printf("\n");
                    }

                    command_found = true;
                    if (exec(path_to_binary.c_str(), argc, argv, NULL, 0) == -1 && errno == ENOENT)
                    {
                        command_found = false;
                    }
                    splitter.clear();
                }
            }
            closedir(d);
        }
        if (!command_found)
        {
            printf("Command not found: %s\n", parser.get_command().data());
        }

    }
    return 0;
}

} // namespace shell
} // namespace msos


REGISTER_APP(msos_shell, &msos::shell::app_start);
