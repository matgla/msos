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

#include <stm32f10x.h>

extern "C"
{

void block()
{
    msos::kernel::process::Scheduler::get().current_process().block();
    SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

void unblock()
{
    msos::kernel::process::Scheduler::get().unblock_all();
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
    semaphore_wait(&value_);
    return true;
}

int Semaphore::post()
{
    semaphore_post(&value_);
    return true;
}

} // namespace synchronization
} // namespace kernel
} // namespace msos

