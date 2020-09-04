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

}

int app_start()
{
    /* TODO: read configuration file */
    read_configuration_file();

    char buffer[100] = {};
    char* argv[10];

    while (std::string_view(buffer).find("exit") == std::string_view::npos)
    {
        write(1, "> \0", 2);
        fgets(buffer, sizeof(buffer), stdin);
        std::string_view request(buffer);

        if (request.find("exit") != std::string_view::npos)
        {
            return 0;
        }

        /* find binary in path environment variable */
        eul::filesystem::path path_to_binary("/rom/bin");
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
        path_to_binary += parser.get_command();
        path_to_cwd += parser.get_command();

        while (!parser.empty())
        {
            argv[argc] = const_cast<char*>(parser.get_next_argument().data());
            ++argc;
        }

        argv[0] = const_cast<char*>(path_to_binary.c_str());

        // TODO: exec must return error code in other way
        if (exec(path_to_cwd.c_str(), argc, argv, NULL, 0) == -2)
        {
            if (exec(path_to_binary.c_str(), argc, argv, NULL, 0) == -2)
            {
                printf("Command not found: \n");
            }
        }
    }

    return 0;
}

} // namespace shell
} // namespace msos


REGISTER_APP(msos_shell, &msos::shell::app_start);
