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

#include "msos/kernel/process/scheduler.hpp"

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

}
