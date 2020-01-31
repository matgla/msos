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

#include "msos/kernel/synchronization/semaphore.hpp"
#include "msos/kernel/synchronization/atomic.hpp"

namespace msos
{
namespace kernel
{
namespace synchronization
{

Semaphore::Semaphore(uint32_t value)
    : value_(value)
{
}

int Semaphore::wait()
{
    int status = 0;
    do
    {
        volatile int copy = atomic::__ldrex(&value_);
        if (copy == 0) continue;
        --copy;
        status = atomic::__strex(copy, &value_);

    } while (status != 0);
    atomic::__dmb();
    return true;
}

int Semaphore::post()
{
    int status = 0;
    do
    {
        volatile int copy = atomic::__ldrex(&value_);
        ++copy;
        status = atomic::__strex(copy, &value_);

    } while (status != 0);
    atomic::__dmb();
    return true;
}

} // namespace synchronization
} // namespace kernel
} // namespace msos

