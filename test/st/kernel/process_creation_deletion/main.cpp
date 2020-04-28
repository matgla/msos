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
#include "msos/kernel/process/context_switch.hpp"

#include "msos/fs/vfs.hpp"
#include "msos/fs/ramfs.hpp"
#include "msos/drivers/character/usart/usart_driver.hpp"
#include "msos/drivers/device_fs.hpp"


UsartWriter writer;

msos::kernel::synchronization::Mutex mutex_;
msos::kernel::synchronization::Mutex stdio_mutex_;

void printa()
{
    stdio_mutex_.lock();
    writer << "Child has additional print" << endl;
    stdio_mutex_.unlock();
}

void b_finish()
{
    stdio_mutex_.lock();
    writer << "B finished" << endl;
    stdio_mutex_.unlock();
}

void child_process_b(void* arg)
{
    hal::time::sleep(std::chrono::milliseconds(10));
    int i = *reinterpret_cast<int*>(arg);
    hal::time::sleep(std::chrono::milliseconds(4));
    stdio_mutex_.lock();
    writer << "Child B " << i << endl;
    stdio_mutex_.unlock();
    b_finish();
}

int i = 0;
void child_fun(void* arg)
{
    /* allow parent to print Parent going to sleep */
    hal::time::sleep(std::chrono::milliseconds(10));
    stdio_mutex_.lock();
    writer << "Child process started" << endl;
    stdio_mutex_.unlock();

    /* allow parent to execute before child */
    hal::time::sleep(std::chrono::milliseconds(15));
    mutex_.lock();
    stdio_mutex_.lock();
    writer << "Child is going to sleep" << endl;
    stdio_mutex_.unlock();
    hal::time::sleep(std::chrono::milliseconds(20));
    stdio_mutex_.lock();
    writer << "Child Done" << endl;
    stdio_mutex_.unlock();

    printa();
    mutex_.unlock();
    int dump = 123;

    spawn(&child_process_b, &i);

    while (i < 2)
    {
        hal::time::sleep(std::chrono::milliseconds(1));
        stdio_mutex_.lock();
        writer << "Child A " << i << ", " << dump << endl;
        stdio_mutex_.unlock();
        i++;
    }

    stdio_mutex_.lock();
    writer << "Child A finished" << endl;
    stdio_mutex_.unlock();

    return;
}

void child_process_c(void *arg)
{
    int i = *reinterpret_cast<int*>(arg);
    int x = 0;
    stdio_mutex_.lock();
    writer << "Child C is starting: " << i << endl;
    stdio_mutex_.unlock();
    while (x < 3)
    {
        stdio_mutex_.lock();
        writer << "Child C is working " << x << endl;
        stdio_mutex_.unlock();
        hal::time::sleep(std::chrono::milliseconds(2));
        x++;
    }

    /* make some parent prints */
    hal::time::sleep(std::chrono::milliseconds(10));
    stdio_mutex_.lock();
    writer << "[TEST DONE]" << endl;
    stdio_mutex_.unlock();
}

void kernel_process(void *arg)
{
    writer << "Hello from Kernel" << endl;
    msos::process::change_context_switch_period(std::chrono::milliseconds(1));

    spawn(&child_fun, NULL);

    stdio_mutex_.lock();
    writer << "Parent" << endl;
    stdio_mutex_.unlock();

    mutex_.lock();

    stdio_mutex_.lock();
    writer << "Parent going to sleep" << endl;
    stdio_mutex_.unlock();

    hal::time::sleep(std::chrono::milliseconds(20));

    stdio_mutex_.lock();
    writer << "Parent Done" << endl;
    stdio_mutex_.unlock();

    mutex_.unlock();

    int i = 0;
    while (i < 11) {
        hal::time::sleep(std::chrono::milliseconds(10));
        stdio_mutex_.lock();
        writer << "Parent: " << hex << i << endl;
        stdio_mutex_.unlock();
        ++i;
        if (i == 10)
        {
            spawn(&child_process_c, &i);

            stdio_mutex_.lock();
            writer << "Parent continues " << i << endl;
            stdio_mutex_.unlock();
        }
    }

    while (true) {}
}

int main()
{
    board::board_init();
    hal::core::Core::initializeClocks();
    msos::fs::Vfs& vfs = msos::fs::Vfs::instance();
    msos::fs::RamFs ramfs;

    vfs.mount_fs("/", &ramfs);
    ramfs.mkdir("dev", 1);

    msos::drivers::character::UsartDriver usart(0);
    msos::drivers::DeviceFs& devfs = msos::drivers::DeviceFs::get_instance();
    devfs.register_driver("tty1", usart);
    vfs.mount_fs("/dev", &devfs);

    for (auto& driver : devfs.get_drivers())
    {
        driver.driver()->load();
    }

    writer << "[TEST START]" << endl;

    spawn_root_process(&kernel_process, NULL, 1024);

    while (true)
    {
    }

}

