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
#include <hal/core/core.hpp>
#include <hal/time/sleep.hpp>
#include <hal/time/time.hpp>

#include "msos/fs/romfs.hpp"
#include "msos/drivers/storage/ram_block_device.hpp"
#include "msos/fs/ramfs.hpp"
#include "msos/fs/vfs.hpp"
#include "msos/fs/mount_points.hpp"
#include "msos/kernel/process/spawn.hpp"
#include "msos/kernel/process/scheduler.hpp"
#include "msos/syscalls/syscalls.hpp"
#include "msos/usart_printer.hpp"
#include "msos/libc/printf.hpp"
#include "msos/apps/app_registry.hpp"

extern "C"
{
extern uint32_t _fs_flash_start;
}

msos::fs::RamFs ramfs;


static UsartWriter writer;

void aa()
{
    writer << "aa" << endl;
}

void kernel_process(void*)
{
    writer << "Starting" << endl;
    msos::fs::Vfs& vfs = msos::fs::Vfs::instance();
    vfs.mount_fs("/", &ramfs);
    ramfs.mkdir("rom", 1);
    ramfs.mkdir("bin", 1);

    uint8_t* romfs_disk = reinterpret_cast<uint8_t*>(&_fs_flash_start);
    msos::fs::RomFs romfs(romfs_disk);
    vfs.mount_fs("/rom", &romfs);

    msos::apps::AppRegistry& apps = msos::apps::AppRegistry::get_instance();
    writer << "apps; 0x" << hex << reinterpret_cast<std::size_t>(&apps) << endl;
    apps.register_executable("aa", reinterpret_cast<std::size_t>(&aa));
    writer << "Mount apps" << endl;
    vfs.mount_fs("/bin", &apps);

    writer << "executin shell" << endl;
    spawn_exec("/bin/msos_shell.bin", NULL, NULL, 0, 2048);

    while (true)
    {
        hal::time::sleep(std::chrono::seconds(1));
    }
}

int main()
{
    hal::core::Core::initializeClocks();
    using Usart = board::interfaces::Usart1;
    Usart::init(9600);

    Usart::on_data([](const uint8_t c)
    {
        write_to_stdin(c);
    });

    spawn_root_process(&kernel_process, NULL, 2048);

    while (true)
    {
    }

}

