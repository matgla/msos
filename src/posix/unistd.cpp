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

#include "unistd.h"

#include <cstring>
#include <string_view>

#include <hal/time/sleep.hpp>

#include "msos/kernel/process/scheduler.hpp"
#include "msos/fs/vfs.hpp"

extern "C"
{

char *getcwd(char *buf, size_t size)
{
    const auto* scheduler = msos::kernel::process::Scheduler::get();
    const auto* process = scheduler->current_process();
    std::string_view cwd = process->get_cwd();
    std::size_t size_to_copy = (size - 1) > cwd.size() ? cwd.size() : size - 1;
    std::memcpy(buf, cwd.data(), size_to_copy);
    buf[size_to_copy] = 0;
    return buf;
}

int chdir(const char* path)
{
    auto& vfs = msos::fs::Vfs::instance();

    eul::filesystem::path path_parser(path);
    std::unique_ptr<msos::fs::IFile> file = vfs.get(path_parser.lexically_normal().c_str(), O_RDONLY);
    if (!file)
    {
        // TODO: move to client code
        printf("File not exists: %s\n", path_parser.lexically_normal().c_str());
        return -1;
    }

    struct stat s;
    file->stat(s);
    if (s.st_mode & S_IFDIR)
    {
        auto* scheduler = msos::kernel::process::Scheduler::get();
        auto* process = scheduler->current_process();
        return process->set_cwd(path_parser.lexically_normal().c_str()) ? 0 : -1;
    }

    // TODO: move to client code
    printf("File is not directory: %s\n", path_parser.lexically_normal().c_str());
    return -1;
}

    unsigned int sleep(unsigned int seconds)
    {
        hal::time::sleep(std::chrono::seconds(seconds));
        return 0;
    }

}
