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
#include "msos/fs/mount_points.hpp"

// #include "msos/fs/"

hal::UsartWriter writer;

void kernel_process()
{
    writer << "I am starting" << endl;

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
}

int main()
{
    hal::core::Core::initializeClocks();
    using LED = board::gpio::LED_BLUE;
    LED::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    using Usart = board::interfaces::Usart1;
    Usart::init(9600);
    hal::time::Time::init();

    root_process(reinterpret_cast<std::size_t>(&kernel_process));

    while (true)
    {
    }

}

