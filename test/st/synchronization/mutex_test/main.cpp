// This file is part of MSOS project. This is simple OS for embedded development devices.
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

#include <board.hpp>

#include <hal/core/core.hpp>
#include <hal/time/sleep.hpp>
#include <hal/time/time.hpp>


#include "msos/usart_printer.hpp"
#include "msos/kernel/process/process.hpp"
#include "msos/kernel/process/process_manager.hpp"
#include "msos/kernel/process/scheduler.hpp"

#include "msos/kernel/synchronization/semaphore.hpp"
#include "msos/kernel/synchronization/mutex.hpp"

#include <msos/kernel/process/spawn.hpp>
#include <msos/kernel/process/context_switch.hpp>

UsartWriter writer;

msos::kernel::synchronization::Mutex mutex_;
msos::kernel::synchronization::Mutex stdio_mutex_;


void child_process(void *arg)
{
    /* wait before parent start */
    hal::time::sleep(std::chrono::milliseconds(10));
    stdio_mutex_.lock();
    writer << "Child is starting" << endl;
    stdio_mutex_.unlock();

    hal::time::sleep(std::chrono::milliseconds(10));

    mutex_.lock();
    stdio_mutex_.lock();
    writer << "Child is going to sleep" << endl;
    stdio_mutex_.unlock();
    hal::time::sleep(std::chrono::milliseconds(15));
    stdio_mutex_.lock();
    writer << "Child finished waiting" << endl;
    stdio_mutex_.unlock();

    mutex_.unlock();
    while (true)
    {
        hal::time::sleep(std::chrono::milliseconds(15));
        stdio_mutex_.lock();
        writer << "Child is working" << endl;
        writer << "[TEST DONE]" << endl;
        stdio_mutex_.unlock();
    }
}

void kernel_process(void* arg)
{
    mutex_.lock();
    msos::process::change_context_switch_period(std::chrono::milliseconds(5));
    spawn(&child_process, NULL);
    stdio_mutex_.lock();
    writer << "Parent is starting" << endl;
    writer << "Parent is going to sleep" << endl;
    stdio_mutex_.unlock();

    hal::time::sleep(std::chrono::milliseconds(20));
    stdio_mutex_.lock();
    writer << "Parent finished waiting" << endl;
    stdio_mutex_.unlock();

    mutex_.unlock();
    /* wait before child start */
    hal::time::sleep(std::chrono::milliseconds(5));
    stdio_mutex_.lock();
    writer << "Parent is working" << endl;
    stdio_mutex_.unlock();
    while (true)
    {
    }
}

int main()
{
    board::board_init();
    const auto& usart = board::interfaces::usarts[0];
    usart->init(9600);

    writer << "[TEST START]" << endl;

    spawn_root_process(&kernel_process, NULL, 1024);

    while (true)
    {
    }

}

