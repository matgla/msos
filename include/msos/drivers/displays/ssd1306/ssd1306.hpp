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

constexpr uint8_t SSD1306_DEFAULT_ADDRESS = 0x3C;

class SSD1306_I2C : public msos::drivers::IDriver
{
public:
    SSD1306_I2C(hal::interfaces::I2C& i2c,
        const uint8_t address = SSD1306_DEFAULT_ADDRESS);

    void load() override;

    void unload() override;

    bool busy();

    constexpr uint32_t height() const
    {
        return 64;
    }

    constexpr uint32_t width() const
    {
        return 128;
    }

    void setPixel(uint16_t x, uint16_t y);

    void clear();

    void display(const gsl::span<uint8_t>& buffer);

    void write(const uint8_t byte);
    void write(gsl::span<const char> buffer);

    std::unique_ptr<fs::IFile> file(std::string_view path, int flags) override;

private:
    void setHome();
    void sendCommand(const uint8_t command);

    hal::interfaces::I2C& i2c_;
    const uint8_t address_;
    std::unique_ptr<uint8_t> buffer_;
};

namespace ssd1306
{

inline SSD1306_I2C create_ssd1306(hal::interfaces::I2C& i2c, const uint8_t address)
{
    return std::move(SSD1306_I2C(i2c, address));
}

}

} // namespace displays
} // namespace drivers
} // namespace msos
