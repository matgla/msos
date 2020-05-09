// This file is part of MSOS project. This is simple OS for embedded development devices.
// Copyright (C) 2020 Mateusz Stadnik
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

#include "msos/drivers/displays/sfml/sfml.hpp"

#include <cstring>

#include <iostream>
#include <thread>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <hal/time/sleep.hpp>

#include "msos/usart_printer.hpp"
#include "msos/drivers/displays/sfml/sfml_file.hpp"

namespace msos
{
namespace drivers
{
namespace displays
{

static UsartWriter writer;

SfmlDisplay::SfmlDisplay()
    // : window_(nullptr)
{
    screen_.create(128, 64);
    // run the program as long as the window is open
}

void SfmlDisplay::load()
{
    std::cerr << "Loading display" << std::endl;
    thread_ = std::make_unique<std::thread>([this]{
        sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            buffer_mutex_.lock();
            sf::Texture tex;
            tex.loadFromImage(screen_);
            sf::Sprite spr;
            spr.setTexture(tex);
            window.draw(spr);
            buffer_mutex_.unlock();
            window.display();
        }
    });
}

bool SfmlDisplay::busy()
{
    return false;
}

void SfmlDisplay::unload()
{
}

void SfmlDisplay::clear()
{
}

void SfmlDisplay::display(const gsl::span<uint8_t>& buffer)
{
    UNUSED1(buffer);
}

void SfmlDisplay::write(const uint8_t byte)
{

    UNUSED1(byte);
}

void SfmlDisplay::write(gsl::span<const char> buffer)
{
    UNUSED1(buffer);
    int x = 0;
    int y = 0;
    buffer_mutex_.lock();
    for (auto byte : buffer)
    {
        if (x == 128)
        {
            y += 8;
            x = 0;
        }
        for (int bit = 0; bit < 8; ++bit)
        {

            if (byte & (1 << bit))
            {
                screen_.setPixel(x, y+bit, sf::Color::White);
            }
        }
        x++;
    }
    buffer_mutex_.unlock();
}

std::unique_ptr<fs::IFile> SfmlDisplay::file(std::string_view path)
{
    return std::make_unique<fs::SfmlFile>(*this, path);
}

} // namespace displays
} // namespace drivers
} // namespace msos



