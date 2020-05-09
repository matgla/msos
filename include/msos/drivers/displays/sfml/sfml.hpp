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

#pragma once

#include <cstdint>
#include <thread>
#include <mutex>
#include <SFML/Graphics.hpp>

#include <board.hpp>
#include <hal/time/sleep.hpp>

#include <eul/utils/unused.hpp>

#include "msos/drivers/i_driver.hpp"

namespace msos
{
namespace drivers
{
namespace displays
{

class SfmlDisplay : public msos::drivers::IDriver
{
public:
    SfmlDisplay();

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

    std::unique_ptr<fs::IFile> file(std::string_view path) override;

private:

    std::mutex buffer_mutex_;
    std::unique_ptr<uint8_t> buffer_;
    sf::Image screen_;
    // std::unique_ptr<sf::RenderWindow> window_;
    std::unique_ptr<std::thread> thread_;
};

namespace sfml
{

inline SfmlDisplay create_sfml()
{
    return SfmlDisplay();
}

}

} // namespace displays
} // namespace drivers
} // namespace msos
