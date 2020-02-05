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

#include "msos/kernel/process/scheduler.hpp"

#include <cstdio>

#include <stm32f10x.h>

extern "C"
{

void block()
{
    printf("Block %d\n", msos::kernel::process::scheduler->current_process().pid());
    msos::kernel::process::scheduler->current_process().block();
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void unblock()
{
    printf("Unblock %d\n", msos::kernel::process::scheduler->current_process().pid());
    msos::kernel::process::scheduler->unblock_all();
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

}

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
    printf("wait\n");
    asm volatile inline("wait_loop:\n\t"
        "ldrex r1, [%[from]]\n\t"
        "cmp r1, #0\n\t"
        "beq block\n\t"
        "sub r1, #1\n\t"
        "strex r2, r1, [%[from]]\n\t"
        "cmp r2, #0\n\t"
        "bne wait_loop\n\t"
        "dmb\n\t"
        :
        : [from] "r" (&value_));

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

    return true;
}

} // namespace synchronization
} // namespace kernel
} // namespace msos

