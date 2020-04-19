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

#pragma once

#include <cstdint>

#include <board.hpp>
#include <hal/time/sleep.hpp>

#include <eul/utils/unused.hpp>

#include "msos/drivers/i_driver.hpp"
#include "msos/drivers/displays/ssd1306/ssd1306_file.hpp"

namespace msos
{
namespace drivers
{
namespace displays
{

constexpr uint8_t SSD1306_SET_LOWER_COLUMN_START_ADDRESS = 0x00;
constexpr uint8_t SSD1306_SET_HIGHER_COLUMN_START_ADDRESS = 0x10;
constexpr uint8_t SSD1306_SET_COLUMN_ADDRESS = 0x21;
constexpr uint8_t SSD1306_SET_PAGE_ADDRESS = 0x22;
constexpr uint8_t SSD1306_SET_MEMORY_ADDRESSING_MODE = 0x20;
constexpr uint8_t SSD1306_SET_DISPLAY_START_LINE = 0x40;
constexpr uint8_t SSD1306_SET_CONTRAST_CONTROL_FOR_BANK0 = 0x81;
constexpr uint8_t SSD1306_SET_SEGMENT_REMAP_1 = 0xA0;
constexpr uint8_t SSD1306_SET_SEGMENT_REMAP_2 = 0xA1;
constexpr uint8_t SSD1306_ENTIRE_DISPLAY_ON_RESUME = 0xA4;
constexpr uint8_t SSD1306_ENTIRE_DISPLAY_ON = 0xA5;
constexpr uint8_t SSD1306_SET_NORMAL_DISPLAY = 0xA6;
constexpr uint8_t SSD1306_SET_INVERSE_DISPLAY = 0xA7;
constexpr uint8_t SSD1306_SET_MULTIPLEX_RATIO = 0xA8;
constexpr uint8_t SSD1306_SET_DISPLAY_ON = 0xAF;
constexpr uint8_t SSD1306_SET_DISPLAY_OFF = 0xAE;
constexpr uint8_t SSD1306_SET_PAGE_START_ADDRESS = 0xB0;
constexpr uint8_t SSD1306_SET_COM_OUTPUT_SCAN_INCREMENTAL = 0xC0;
constexpr uint8_t SSD1306_SET_COM_OUTPUT_SCAN_DECREMENTAL = 0xC8;
constexpr uint8_t SSD1306_SET_DISPLAY_OFFSET = 0xD3;
constexpr uint8_t SSD1306_SET_DISPLAY_CLOCK_DIVIDE_RATIO = 0xD5;
constexpr uint8_t SSD1306_SET_PRE_CHARGE_PERIOD = 0xD9;
constexpr uint8_t SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION = 0xDA;
constexpr uint8_t SSD1306_SET_VCOMH_DESELECT_LEVEL = 0xDB;
constexpr uint8_t SSD1306_NOP = 0xE3;
constexpr uint8_t SSD1306_CHARGE_PUMP = 0x8D;

constexpr uint8_t SSD1306_DEFAULT_ADDRESS = 0x3C;

class SSD1306_I2C : public msos::drivers::IDriver
{
public:
    SSD1306_I2C(hal::interfaces::I2C& i2c,
        const uint8_t address = SSD1306_DEFAULT_ADDRESS)
        : i2c_(i2c)
        , address_(address)
    {
    }

    void load() override
    {
        hal::time::sleep(std::chrono::milliseconds(100));
        i2c_.init();

        sendCommand(SSD1306_SET_DISPLAY_OFF);
        sendCommand(SSD1306_SET_MULTIPLEX_RATIO);
        sendCommand(0x3f);
        sendCommand(SSD1306_SET_DISPLAY_OFFSET);
        sendCommand(0x00);
        sendCommand(SSD1306_SET_DISPLAY_START_LINE);
        sendCommand(SSD1306_SET_SEGMENT_REMAP_2);
        sendCommand(SSD1306_SET_COM_OUTPUT_SCAN_DECREMENTAL);
        sendCommand(SSD1306_SET_COM_PINS_HARDWARE_CONFIGURATION);
        sendCommand(0x12);
        sendCommand(SSD1306_SET_CONTRAST_CONTROL_FOR_BANK0);
        sendCommand(0xFF);
        sendCommand(SSD1306_ENTIRE_DISPLAY_ON_RESUME);

        sendCommand(SSD1306_SET_NORMAL_DISPLAY);

        sendCommand(SSD1306_SET_DISPLAY_CLOCK_DIVIDE_RATIO);
        sendCommand(0xF0);

        sendCommand(SSD1306_CHARGE_PUMP);
        sendCommand(0x14);

        sendCommand(SSD1306_SET_MEMORY_ADDRESSING_MODE);
        sendCommand(0x00);


        sendCommand(SSD1306_SET_PRE_CHARGE_PERIOD);
        sendCommand(0x14);
        // sendCommand(SSD1306_SET_VCOMH_DESELECT_LEVEL);
        // sendCommand(0x20);
        sendCommand(SSD1306_SET_DISPLAY_ON);

        clear();
    }

    void unload() override
    {
    }

    constexpr uint32_t height() const
    {
        return 64;
    }

    constexpr uint32_t width() const
    {
        return 128;
    }

    void setPixel(uint16_t x, uint16_t y)
    {
        UNUSED2(x, y);
        // sendCommand(SSD1306_SET_COLUMN_ADDRESS);
        // sendCommand(x);
        // sendCommand(x);

        // sendCommand(SSD1306_SET_PAGE_ADDRESS);
        // sendCommand(y/8);
        // sendCommand(y/8);

        // i2c_.start(address_ | 0x01);
        // uint8_t old_byte = i2c_.read();
        // old_byte = i2c_.read();
        // using Serial = board::interfaces::SERIAL;
        // char data[20];
        // Serial::write("Requesting x: ");
        // itoa(x, data, 10);
        // Serial::write(data);
        // Serial::write(", y: ");
        // itoa(y, data, 10);
        // Serial::write(data);
        // Serial::write("\n");

        // itoa(old_byte, data, 10);
        // Serial::write("Old: ");
        // Serial::write(data);
        // Serial::write("\n");
        // i2c_.stop();

        // i2c_.start(address_);
        // i2c_.write(SSD1306_SET_DISPLAY_START_LINE);
        // i2c_.write(old_byte | (1 << (y % 8)));
        // i2c_.stop();
    }
    void clear()
    {
        setHome();
        for (int x = 0; x < 128; x++)
        {
            for (int page = 0; page < 8; page++)
            {
                i2c_.start(address_);
                i2c_.write(SSD1306_SET_DISPLAY_START_LINE);
                i2c_.write(0);
                i2c_.stop();
            }
        }
    }
    void display(const gsl::span<uint8_t>& buffer)
    {
        setHome();

        for (uint8_t packet = 0; packet < buffer.size()/16; packet++)
        {
            i2c_.start(address_);
            i2c_.write(SSD1306_SET_DISPLAY_START_LINE);
            for (uint8_t packet_byte = 0; packet_byte < 16; ++packet_byte)
            {
                i2c_.write(buffer[packet*16+packet_byte]);
            }
            i2c_.stop();
        }
    }

    void write(const uint8_t byte)
    {
        i2c_.start(address_);
        i2c_.write(SSD1306_SET_DISPLAY_START_LINE);
        i2c_.write(byte);
        i2c_.stop();
    }

    virtual std::unique_ptr<fs::IFile> file(std::string_view path)
    {
        return std::make_unique<fs::Ssd1306File>(*this, path);
    }

private:
    void setHome()
    {
        sendCommand(SSD1306_SET_COLUMN_ADDRESS);
        sendCommand(0x00);
        sendCommand(0x7F);

        sendCommand(SSD1306_SET_PAGE_ADDRESS);
        sendCommand(0x00);
        sendCommand(0x07);
    }

    void sendCommand(const uint8_t command)
    {
        i2c_.start(address_);
        i2c_.write(0x00);
        i2c_.write(command);
        i2c_.stop();
    }

    hal::interfaces::I2C& i2c_;
    const uint8_t address_;
};

} // namespace displays
} // namespace drivers
} // namespace msos
