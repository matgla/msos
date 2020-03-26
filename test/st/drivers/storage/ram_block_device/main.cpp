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

#include <cstring>

#include <board.hpp>

#include <hal/time/sleep.hpp>
#include <hal/core/core.hpp>
#include <hal/time/time.hpp>

#include "msos/usart_printer.hpp"

#include "msos/drivers/storage/ram_block_device.hpp"

UsartWriter writer;

int main()
{
    hal::core::Core::initializeClocks();
    using LED = board::gpio::LED_BLUE;
    LED::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    using Usart = board::interfaces::Usart1;
    Usart::init(9600);

    writer << "[TEST_START]" << endl;

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

    char buffer[24];
    gsl::span<uint8_t> buffer_span = gsl::make_span(reinterpret_cast<uint8_t*>(buffer), 24);

    std::strncpy(buffer, "Hello", sizeof(buffer));

    writer << "Erase 24 bytes at 0x0" << endl;
    error = bd.erase(0x0, 24);
    if (error)
    {
        writer << "Can't erase" << endl;
    }

    writer << "Write Hello at address 0x4" << endl;
    error = bd.write(0x04, buffer_span);

    if (error)
    {
        writer << "Can't write to bd" << endl;
    }

    std::memset(buffer, 0, 24);

    writer << "Read" << endl;
    error = bd.read(0x0, buffer_span);
    if (error)
    {
        writer << "Can't read from bd" << endl;
    }

    for (char byte : buffer)
    {
        writer << hex << static_cast<int>(byte) << " : ";
        writer << byte << endl;
    }

    writer << "[TEST DONE]" << endl;

    while (true)
    {
    }

}

