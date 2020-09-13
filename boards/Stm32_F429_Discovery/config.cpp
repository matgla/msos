// This file is part of MS Keychain Gamer project. This is tiny game console.
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

#include <config.hpp>

#include <arch/armv7-m/normal_scheduler.hpp>
#include <msos/kernel/process/scheduler.hpp>

#include <msos/posix/fsync.hpp>

extern "C"
{
    int fsync(int fd)
    {
        return _fsync(fd);
    }
}

namespace msos
{

namespace
{
    msos::arch::armv7m::NormalScheduler scheduler;
}

void system_config()
{
    kernel::process::Scheduler::set(&scheduler);
}

}
