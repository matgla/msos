// This file is part of MSOS project. This is simple OS for embedded development devices.
// Copyright (C) 2019 Mateusz Stadnik
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

#include <cstdint>
#include <filesystem>

#include <board.hpp>
#include <config.hpp>
#include <hal/core/core.hpp>
#include <hal/time/sleep.hpp>
#include <hal/time/time.hpp>
#include <hal/interrupt/systick.hpp>

#include "msos/kernel/process/context_switch.hpp"
#include "msos/fs/romfs.hpp"
#include "msos/drivers/storage/ram_block_device.hpp"
#include "msos/drivers/character/usart/usart_driver.hpp"
#include "msos/drivers/device_fs.hpp"
#include "msos/fs/ramfs.hpp"
#include "msos/fs/vfs.hpp"
#include "msos/fs/mount_points.hpp"
#include "msos/kernel/process/spawn.hpp"
#include "msos/kernel/process/scheduler.hpp"
#include "msos/syscalls/syscalls.hpp"
#include "msos/usart_printer.hpp"
#include "msos/libc/printf.hpp"
#include "msos/apps/app_registry.hpp"
#include "msos/drivers/displays/ssd1306/ssd1306.hpp"
#include "msgui/Factory.hpp"
#include "msgui/policies/chunk/SSD1308ChunkPolicy.hpp"
#include "msgui/policies/data/DefaultMemoryPolicy.hpp"
#include "msgui/fonts/Font5x7.hpp"

extern "C"
{
extern uint32_t _fs_flash_start;
}

msos::fs::RamFs ramfs;
UsartWriter writer;

void kernel_process(void*)
{
    msos::fs::Vfs& vfs = msos::fs::Vfs::instance();
    vfs.mount_fs("/", &ramfs);
    ramfs.mkdir("rom", 1);
    ramfs.mkdir("bin", 1);
    ramfs.mkdir("dev", 1);

    if (_fs_flash_start != 0)
    {
        uint8_t* romfs_disk = reinterpret_cast<uint8_t*>(&_fs_flash_start);
        msos::fs::RomFs romfs(romfs_disk);
        vfs.mount_fs("/rom", &romfs);
    }

    // writer << "Start" << endl;
    msos::drivers::DeviceFs& devfs = msos::drivers::DeviceFs::get_instance();

    for (auto& driver : devfs.get_drivers())
    {
        driver.driver()->load();
        writer << "Driver loaded: " << driver.path() << endl;
    }

    vfs.mount_fs("/dev", &devfs);

    msos::apps::AppRegistry& apps = msos::apps::AppRegistry::get_instance();
    vfs.mount_fs("/bin", &apps);

    exec("/bin/msos_shell.bin", NULL, NULL, 0);

    while (true)
    {
        yield();
    }
}

int main()
{
    board::board_init();
    // hal::core::Core::initializeClocks();

    auto usart = board::interfaces::usarts()[0];
    usart->init(9600);
    usart->write("HEJOTRA\n");

    msos::system_config();

    spawn_root_process(&kernel_process, NULL, 4048);

    while (true)
    {
    }

}

