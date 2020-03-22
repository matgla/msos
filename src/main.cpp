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

#include <board.hpp>
#include <hal/time/sleep.hpp>
#include <hal/core/core.hpp>

#include <hal/time/time.hpp>

#include "msos/usart_printer.hpp"
#include "msos/kernel/process/process.hpp"
#include "msos/drivers/storage/ram_block_device.hpp"
#include "msos/fs/ramfs.hpp"
#include "msos/fs/romfs.hpp"
#include "msos/fs/mount_points.hpp"

#include "msos/kernel/process/spawn.hpp"
#include "msos/kernel/process/process.hpp"
#include "msos/kernel/process/process_manager.hpp"

#include "msos/kernel/synchronization/semaphore.hpp"
#include "msos/kernel/synchronization/mutex.hpp"

#include <msos/dynamic_linker/dynamic_linker.hpp>
#include <msos/dynamic_linker/environment.hpp>

// #include "msos/fs/"

msos::dl::DynamicLinker dynamic_linker;
hal::UsartWriter writer;

uint32_t get_lot_at(uint32_t address)
{
    writer << "Getting address of LOT: 0x" << hex << address << endl;
    return dynamic_linker.get_lot_for_module_at(address);
}

extern "C"
{

extern uint32_t _fs_flash_start;
extern uint32_t _fs_flash_end;

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


void kernel_process(void*)
{
    writer << "I am starting" << endl;

    uint32_t fs_flash_start = reinterpret_cast<uint32_t>(&_fs_flash_start);
    uint32_t fs_flash_end = reinterpret_cast<uint32_t>(&_fs_flash_end);

    uint32_t fs_flash_size =  fs_flash_end > fs_flash_start ? fs_flash_end - fs_flash_start : fs_flash_start - fs_flash_end ;
    writer << "FS flash start: 0x" << hex << fs_flash_start << ", with length: " << fs_flash_size << endl;

    uint8_t* romfs_disk = reinterpret_cast<uint8_t*>(0x08008000);

    msos::drivers::storage::RamBlockDevice bd(2048, 1, 1, 1);

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

    msos::fs::RamFs ramfs;
    msos::fs::mount_points.mount_filesystem("/", &ramfs);

    FILE* test_file = fopen("/test.txt", "w");

    if (test_file == NULL)
    {
        writer << "File is null" << endl;
    }
    else
    {
        writer << "File is not null" << endl;
        fputs("Hej, ten plik dostaje dane i nawet je zapisuje :)\n", test_file);

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
    writer << "Size: " << dec << sizeof(msos::fs::RomFs) << endl;
    writer << "RomFs addr: 0x" << hex << reinterpret_cast<uint32_t>(&romfs) << endl;

    msos::fs::mount_points.mount_filesystem("/rom", &romfs);

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
            writer << endl;
        }
        fclose(romfs_file);
    }


    if (fork())
    {
        writer << "Spawned child process" << endl;
    }
    else
    {
         /* get raw file and execute */
    auto file = romfs.get("/interface_and_classes.bin");
    if (!file)
    {
        writer << "Can't open file" << endl;
        exit(-1);
    }
    trap();
    std::size_t module_address = reinterpret_cast<uint32_t>(file->data().data());
    writer << "Got file with binary: " << hex << module_address << endl;
    writer << "Got file with binary: " << file->name() << endl;

    uint32_t address_of_lot_getter = reinterpret_cast<uint32_t>(&get_lot_at);
    uint32_t* lot_in_memory = reinterpret_cast<uint32_t*>(0x20000000);
    *lot_in_memory = address_of_lot_getter;
    writer << "Address of lot getter: 0x" << hex << address_of_lot_getter << endl;
    writer << "Address of lot in memory: 0x" << hex << reinterpret_cast<uint32_t>(lot_in_memory) << endl;

    int extern_data = 123;
    msos::dl::Environment<4> env{
        msos::dl::SymbolAddress{"usart_write", &usart_write},
        msos::dl::SymbolAddress{"strlen", &strlen},
        msos::dl::SymbolAddress{"write", &write},
        msos::dl::SymbolAddress{"extern_1", &extern_data}
    };
    writer << "[TEST START]" << endl;

    const msos::dl::LoadedModule* module = dynamic_linker.load_module(module_address, msos::dl::LoadingModeCopyData, env);

    if (module == nullptr)
    {
        writer << "Module not loaded properly" << endl;
        writer << "[TEST DONE]" << endl;
        while (true)
        {
        }
    }
    writer << "Module loaded" << endl;

    module->execute();

    writer << "[TEST DONE]" << endl;
        exit(0);
    }


    while (true)
    {
        hal::time::sleep(std::chrono::seconds(1));
        writer << "Parent" << endl;
    }
}

int main()
{
    hal::core::Core::initializeClocks();
    using LED = board::gpio::LED_BLUE;
    LED::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    using Usart = board::interfaces::Usart1;
    Usart::init(9600);

    spawn_root_process(&kernel_process, NULL);

    while (true)
    {
    }

}

