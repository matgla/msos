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

#include <cstdlib>
#include <cstring>

#include <board.hpp>
#include <hal/time/sleep.hpp>
#include <hal/core/core.hpp>

#include <hal/time/time.hpp>

#include "msos/usart_printer.hpp"
#include "msos/kernel/process/process.hpp"
#include "msos/drivers/storage/ram_block_device.hpp"
#include "msos/fs/ramfs.hpp"
#include "msos/fs/romfs.hpp"
#include "msos/fs/vfs.hpp"
#include "msos/fs/mount_points.hpp"

#include "msos/kernel/process/process.hpp"
#include "msos/kernel/process/process_manager.hpp"
#include "msos/kernel/process/scheduler.hpp"

#include "msos/kernel/synchronization/semaphore.hpp"
#include "msos/kernel/synchronization/mutex.hpp"

#include <msos/dynamic_linker/dynamic_linker.hpp>
#include <msos/dynamic_linker/environment.hpp>

#include <msos/kernel/process/spawn.hpp>

// #include "msos/fs/"

UsartWriter writer;

extern "C"
{

extern uint32_t _fs_flash_start;

void usart_write(const char* data);

}

void usart_write(const char* data)
{
    writer << data;
}


void trap()
{
    writer << "Trap" << endl;
}

void child_process(void* arg)
{
    writer << "Welcome in child process with arg " << reinterpret_cast<uint32_t>(arg);
    auto& romfs = *reinterpret_cast<msos::fs::RomFs*>(arg);
    /* get raw file and execute */
    auto file = romfs.get("/interface_and_classes.bin");
    writer << "Get file" << endl;
    if (!file)
    {
        writer << "Can't open file" << endl;
        return;
    }
    trap();
    std::size_t module_address = reinterpret_cast<uint32_t>(file->data());
    writer << "Got file with binary: " << hex << module_address << endl;
    writer << "Got file with binary: " << file->name() << endl;

    int extern_data = 123;
    msos::dl::Environment<4> env{
        msos::dl::SymbolAddress{"usart_write", &usart_write},
        msos::dl::SymbolAddress{"strlen", &strlen},
        msos::dl::SymbolAddress{"write", &write},
        msos::dl::SymbolAddress{"extern_1", &extern_data}
    };

    exec("/rom/interface_and_classes.bin", NULL, reinterpret_cast<const SymbolEntry*>(env.data().data()), env.data().size());

    writer << "[TEST DONE]" << endl;
}

void kernel_process(void *)
{
    uint32_t fs_flash_start = reinterpret_cast<uint32_t>(&_fs_flash_start);

    uint8_t* romfs_disk = reinterpret_cast<uint8_t*>(fs_flash_start);

    msos::drivers::storage::RamBlockDevice bd(128, 1, 1, 1);

    int error = bd.init();
    if (error)
    {
        writer << "Cannot initialize block device" << endl;
    }

    writer << "Device geometry: " << endl
        << "Size: " << bd.size() << endl
        << "Read size: " << bd.read_size() << endl
        << "Write size: " << bd.write_size() << endl
        << "Erase size: " << bd.erase_size() << endl;

    msos::fs::Vfs vfs;
    msos::fs::RamFs ramfs;
    vfs.mount_fs("/", &ramfs);
    msos::fs::mount_points.mount_filesystem("/", &vfs);

    writer << "Creating file with name: /test.txt" << endl;
    FILE* test_file = fopen("/test.txt", "w");

    if (test_file == NULL)
    {
        writer << "File is null" << endl;
    }
    else
    {
        writer << "File is not null" << endl;
        fputs("Storing some data in file :)\n", test_file);

        fclose(test_file);
    }

    test_file = fopen("/test.txt", "r");

    if (test_file == NULL)
    {
        writer << "File is null" << endl;
    }
    else
    {
        if (test_file)
        {
            writer << "Content of file: " << endl;
            int c;
            int i = 0;
            while ((c = getc(test_file)) != EOF)
            {
                writer << static_cast<char>(c);
            }
            writer << endl;
        }
        fclose(test_file);
    }


    writer << "Started testing ROMFS disk" << endl;
    msos::fs::RomFs romfs(romfs_disk);
    vfs.mount_fs("/rom", &romfs);

    writer << "Opening file /rom/test.txt" << endl;
    FILE *romfs_file = fopen("/rom/test.txt", "r");

    writer << "Fopen called" << endl;
    if (romfs_file == NULL)
    {
        writer << "Can't open: /rom/test.txt" << endl;
    }
    else
    {
        if (romfs_file)
        {

            writer << "Content of file: " << endl;
            int c;
            int i = 0;
            while ((c = getc(romfs_file)) != EOF)
            {
                writer << static_cast<char>(c);
            }
            // writer << endl;
        }
        fclose(romfs_file);
    }

    if (spawn(child_process, &romfs))
    {
        writer << "Spawned child process" << endl;
    }

    while (true)
    {
    }
}

int main()
{
    hal::core::Core::initializeClocks();
    using LED = board::gpio::LED_BLUE;
    LED::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    using Usart = board::interfaces::Usart1;
    Usart::init(9600);

    writer << "[TEST START]" << endl;
    spawn_root_process(&kernel_process, NULL, 1024);

    while (true)
    {
    }

}

