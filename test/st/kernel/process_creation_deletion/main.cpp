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

#include <hal/time/sleep.hpp>
#include <hal/core/core.hpp>
#include <hal/time/time.hpp>

#include "msos/usart_printer.hpp"

#include "msos/kernel/process/process.hpp"
#include "msos/kernel/process/process_manager.hpp"
#include "msos/kernel/process/scheduler.hpp"

#include "msos/kernel/synchronization/semaphore.hpp"
#include "msos/kernel/synchronization/mutex.hpp"
#include "msos/kernel/process/spawn.hpp"

hal::UsartWriter writer;

msos::kernel::synchronization::Mutex mutex_;

void printa()
{
    writer << "Child has additional print" << endl;
}

void b_finish()
{
    writer << "B finished" << endl;
}

void trap()
{
    writer << "Trap" << endl;
}

void child_process_b(void* arg)
{
    int i = *reinterpret_cast<int*>(arg);
    while (i < 4)
    {
        hal::time::sleep(std::chrono::milliseconds(500));
        writer << "Child B " << i << endl;
        i++;
    }

    b_finish();
}

void child_fun(void* arg)
{
    writer << "Child process started" << endl;
    mutex_.lock();
    writer << "Child is going to sleep" << endl;
    hal::time::sleep(std::chrono::milliseconds(500));

    writer << "Child Done" << endl;
    printa();
    mutex_.unlock();
    int i = 0;
    int dump = 123;

    spawn(&child_process_b, &i);
    while (i < 2)
    {
        hal::time::sleep(std::chrono::milliseconds(500));
        writer << "Child A " << i << ", " << dump << endl;
        i++;
    }

    writer << "Child A finished" << endl;

    printf("Exit aa\n");

    return;
}

void child_process_c(void *arg)
{
    int i = *reinterpret_cast<int*>(arg);
    int x = 0;
    writer << "Child C is starting: " << i << endl;
    while (x < 3)
    {
        writer << "Child C is working " << x << endl;
        hal::time::sleep(std::chrono::milliseconds(50));
        x++;
    }
    writer << "[TEST DONE]" << endl;
}

void kernel_process()
{
    writer << "Hello from Kernel" << endl;

    spawn(&child_fun, NULL);

    writer << "Parent" << endl;

    mutex_.lock();

    writer << "Parent going to sleep" << endl;

    hal::time::sleep(std::chrono::milliseconds(200));

    writer << "Parent Done" << endl;
    mutex_.unlock();
    int i = 0;
    while (true) {
        hal::time::sleep(std::chrono::milliseconds(100));
        mutex_.lock();
        writer << "Parent: " << hex << i << endl;
        mutex_.unlock();
        ++i;
        if (i == 10)
        {
            writer << "Forking" << endl;
            spawn(&child_process_c, &i);

            writer << "Parent continues " << i << endl;
        }
    }

    while (true) {}
}

int main()
{
    hal::core::Core::initializeClocks();
    using LED = board::gpio::LED_BLUE;
    LED::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    using Usart = board::interfaces::Usart1;
    Usart::init(9600);
    hal::time::Time::init();

    writer << "[TEST START]" << endl;

    root_process(reinterpret_cast<std::size_t>(&kernel_process));

    while (true)
    {
    }

}

