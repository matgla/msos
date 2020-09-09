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

#include "msos/os/sys/ioctl.h"

#include <cstdio>

#include <eul/utils/unused.hpp>

#include "msos/kernel/process/scheduler.hpp"
#include "msos/kernel/process/process.hpp"
#include "msos/fs/i_file.hpp"

int ioctl(int fd, uint32_t cmd, void* arg)
{
    const auto* scheduler = msos::kernel::process::Scheduler::get();
    if (!scheduler)
    {
        return -1;
    }

    const auto* current_process = scheduler->current_process();
    if (!current_process)
    {
        return -1;
    }
    msos::fs::IFile* file = current_process->get_file(fd);
    if (file == nullptr)
    {
        return -1;
    }
    return file->ioctl(cmd, arg);
}
