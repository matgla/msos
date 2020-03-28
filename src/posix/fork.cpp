// // This file is part of MSOS project. This is simple OS for embedded development devices.
// // Copyright (C) 2019 Mateusz Stadnik
// //
// // This program is free software: you can redistribute it and/or modify
// // it under the terms of the GNU General Public License as published by
// // the Free Software Foundation, either version 3 of the License, or
// // (at your option) any later version.
// //
// // This program is distributed in the hope that it will be useful,
// // but WITHOUT ANY WARRANTY; without even the implied warranty of
// // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// // GNU General Public License for more details.
// //
// // You should have received a copy of the GNU General Public License
// // along with this program.  If not, see <https://www.gnu.org/licenses/>.

// #include "unistd.h"

// #include <cstdint>
// #include <cstdio>

// #include "msos/kernel/process/context_switch.hpp"
// #include "msos/kernel/process/process_manager.hpp"
// #include "msos/kernel/process/scheduler.hpp"
// #include "msos/kernel/process/process.hpp"
// #include "msos/kernel/process/registers.hpp"
// #include "msos/syscalls/syscalls.hpp"
// #include "msos/kernel/synchronization/mutex.hpp"

// #include <hal/time/time.hpp>
// #include <hal/core/criticalSection.hpp>
// #include <hal/interrupt/systick.hpp>

// #include <stm32f10x.h>

// extern "C"
// {
//     pid_t getpid();
//     void dump_registers(void);

//     pid_t process_fork(uint32_t sp, uint32_t return_address, msos::kernel::process::RegistersDump* registers);

//     int was_current_process_deleted();
//     void update_stack_pointer(const std::size_t* stack);
//     const std::size_t* get_next_task();
//     void set_first_task_processed();


//     void SysTick_Handler(void);
// }

// static inline uint32_t get_PRIMASK()
// {
//     uint32_t mask;
//     asm inline volatile(
//         "mrs %0, primask\n" : "=r"(mask));
//     return mask;
// }

// extern std::size_t __stack_start;
// std::size_t* stack_start = &__stack_start;
// constexpr std::size_t default_stack_size = 1024;

// msos::kernel::process::ProcessManager processes;

// namespace msos
// {
// namespace kernel
// {
// namespace process
// {


// }
// }
// }



// static bool was_initialized = false;
// static bool first = true;

// int was_current_process_deleted()
// {
//     return msos::kernel::process::Scheduler::get().current_process_was_deleted();
// }



// void set_first_task_processed()
// {
//     first = false;
// }

// const std::size_t* get_next_task()
// {
//     return msos::kernel::process::Scheduler::get().schedule_next();
// }

// pid_t getpid()
// {
//     return msos::kernel::process::Scheduler::get().current_process().pid();
// }

// extern "C"
// {
//     volatile int is_pendsv_blocked = false;
// }

// pid_t root_process(const std::size_t process)
// {
//     auto& root_process = processes.create_process(process, default_stack_size);
//     msos::kernel::process::Scheduler::get().schedule_next();
//     msos::process::initialize_context_switching();

//     while(true)
//     {
//     }
// }

// extern "C"
// {
// volatile uint32_t syscall_return_code = 0;

// }

