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

hal::UsartWriter writer;

static msos::kernel::synchronization::Semaphore stdio_mutex_(1);

msos::kernel::synchronization::Mutex mutex_;

void child_process(void *arg)
{
    writer << "Child is starting" << endl;
    mutex_.lock();
    writer << "Child is going to sleep" << endl;
    hal::time::sleep(std::chrono::milliseconds(150));
    writer << "Child finished waiting" << endl;

    mutex_.unlock();
    while (true)
    {
        hal::time::sleep(std::chrono::milliseconds(150));
        stdio_mutex_.wait();
        writer << "Child is working" << endl;
        writer << "[TEST DONE]" << endl;
        stdio_mutex_.post();
    }
}

void kernel_process(void* arg)
{
    spawn(&child_process, NULL);
    writer << "Parent is starting" << endl;


    mutex_.lock();

    writer << "Parent is going to sleep" << endl;

    hal::time::sleep(std::chrono::milliseconds(200));
    writer << "Parent finished waiting" << endl;
    mutex_.unlock();
    while (true)
    {
        stdio_mutex_.wait();
        writer << "Parent is working" << endl;
        stdio_mutex_.post();
        hal::time::sleep(std::chrono::milliseconds(100));
    }
}

int main()
{
    hal::core::Core::initializeClocks();
    using LED = board::gpio::LED_BLUE;
    LED::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    using Usart = board::interfaces::Usart1;
    Usart::init(115200);

    writer << "[TEST START]" << endl;

    spawn_root_process(&kernel_process, NULL);

    while (true)
    {
    }

}

