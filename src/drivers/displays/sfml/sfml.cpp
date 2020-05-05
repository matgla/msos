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

#include <thread>

#include <SFML/Window.hpp>

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
    : buffer_(new uint8_t[1024])
    , window_(nullptr)
{
    std::memset(buffer_.get(), 0 , 1024);
    // run the program as long as the window is open
}

void SfmlDisplay::load()
{
    window_.reset(new sf::Window(sf::VideoMode(128, 64), "My window"));
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
}

std::unique_ptr<fs::IFile> SfmlDisplay::file(std::string_view path)
{
    return std::make_unique<fs::SfmlFile>(*this, path);
}

} // namespace displays
} // namespace drivers
} // namespace msos



