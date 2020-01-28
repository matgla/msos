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

hal::UsartWriter writer;

static msos::kernel::synchronization::Semaphore mutex(1);

msos::kernel::synchronization::Mutex mutex_;

extern "C"
{
    pid_t _fork();
}

void kernel_process()
{
    printf("Hello from Kernel\n");

    if (_fork())
    {
        writer << "Parent" << endl;

        mutex_.lock();

        writer << "parent going to sleep" << endl;

        hal::time::sleep(std::chrono::seconds(200));

        writer << "Parent Done" << endl;
        mutex_.unlock();
        while (true) {
            hal::time::sleep(std::chrono::milliseconds(100));
            writer << "Parent" << endl;
        }
    }
    else
    {

        writer << "Child" << endl;
        mutex_.lock();
        writer << "Child is going to sleep" << endl;
        hal::time::sleep(std::chrono::seconds(1));

        writer << "Child Done" << endl;
        mutex_.unlock();
        while (true) {
            hal::time::sleep(std::chrono::milliseconds(100));
            writer << "Child" << endl;
        }
    }
}
#include <cstring>
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

