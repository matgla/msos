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

hal::UsartWriter writer;

extern "C"
{
    pid_t _fork();
}

msos::kernel::synchronization::Mutex mutex_;

void printa()
{
    writer << "Child has additional print" << endl;
}

void b_finish()
{
    writer << "B finished" << endl;
}

void child_fun()
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

    if (_fork())
    {
        while (i < 2)
        {
            hal::time::sleep(std::chrono::milliseconds(500));
            writer << "Child A " << i << ", " << dump << endl;
            i++;
        }

        writer << "Child A finished" << endl;
    }
    else
    {
        while (i < 4)
        {
            hal::time::sleep(std::chrono::milliseconds(500));
            writer << "Child B " << i << endl;
            i++;
        }

        b_finish();
    }

    return;
}

void kernel_process()
{
    writer << "Hello from Kernel" << endl;

    if (_fork())
    {
        writer << "Parent" << endl;

        mutex_.lock();

        writer << "Parent going to sleep" << endl;

        hal::time::sleep(std::chrono::seconds(1));

        writer << "Parent Done" << endl;
        mutex_.unlock();
        int i = 0;
        while (true) {
            hal::time::sleep(std::chrono::milliseconds(100));
            writer << "Parent: " << hex << i << endl;
            ++i;
            if (i == 10)
            {
                writer << "Forking" << endl;
                if (_fork())
                {
                    writer << "Parent continues " << i << endl;
                }
                else
                {
                    int x = 0;
                    writer << "Child C is starting: " << i << endl;
                    while (x < 10)
                    {
                        // writer << "Child C is working " << x << endl;
                        hal::time::sleep(std::chrono::milliseconds(50));
                        x++;
                    }
                    writer << "[TEST DONE]" << endl;
                    break;
                }
            }
        }
    }
    else
    {
        child_fun();
    }

    writer << "Some child died" << endl;
    // Child process default exit
    exit(0);

    // Main process should never goes here
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

