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

#include <cstdio>

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
    asm volatile inline 
        ("wait_loop:\n\t"
         "ldrex r1, [%[from]]\n\t"
         "cmp r1, #0\n\t"
         "beq wait_loop\n\t"
         "sub r1, #1\n\t"
         "strex r2, r1, [%[from]]\n\t"
         "cmp r2, #0\n\t"
         "bne wait_loop\n\t"
         "dmb\n\t" 
         : 
         : [from] "r" (&value_)
         );

   // printf("after wait %d\n", value_);
   /* volatile int copy = 0;
    do
    {
        copy = atomic::__ldrex(&value_);
        if (copy == 0) continue;
        --copy;
    } while (atomic::__strex(copy, &value_) != 0);
    atomic::__dmb(); */
    return true;
}

int Semaphore::post()
{
    asm volatile inline(
        "post_loop:\n\t"
        "ldrex r1, [%[from]]\n\t"
        "add r1, #1\n\t"
        "strex r2, r1, [%[from]]\n\t"
        "cmp r2, #0\n\t"
        "bne post_loop\n\t"
        "dmb\n\t"
        :
        : [from] "r" (&value_)
        );

    /*volatile int copy = 0;
    do
    {
        copy = atomic::__ldrex(&value_);
        ++copy;
    } while (atomic::__strex(copy, &value_) != 0);
    atomic::__dmb();*/
    return true;
}

} // namespace synchronization
} // namespace kernel
} // namespace msos

