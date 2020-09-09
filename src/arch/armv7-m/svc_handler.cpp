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

#include <hal/interrupt/svc.hpp>
#include <hal/interrupt/pendsv.hpp>

#include "msos/syscalls/syscalls.hpp"
#include "msos/kernel/synchronization/semaphore.hpp"
#include "msos/kernel/process/context_switch.hpp"

#include "arch/armv7-m/svc_handler.hpp"

namespace msos
{
namespace process
{

void initialize_svc()
{
    printf("Initialization of svc\n");
    hal::interrupt::set_svc_handler([](uint32_t number, void* args, void* out){
        printf("Handling svc call: %d\n", number);
        switch(number)
        {
            case SyscallNumber::SYSCALL_EXIT:
            {
                msos::syscalls::process_exit(*reinterpret_cast<int*>(args));
            } break;
            case SyscallNumber::SYSCALL_START_ROOT_PROCESS:
            {
                switch_to_next_task();
            } break;
            case SyscallNumber::SYSCALL_SEMAPHORE_POST:
            {
                semaphore_post(reinterpret_cast<int*>(args));
            } break;
            case SyscallNumber::SYSCALL_SEMAPHORE_WAIT:
            {
                semaphore_wait(reinterpret_cast<int*>(args));
            } break;
            case SyscallNumber::YIELD:
            {
                hal::interrupt::trigger_pendsv();
            } break;
        }
    });
}

} // namespace process
} // namespace msos
