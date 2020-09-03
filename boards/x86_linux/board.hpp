// This file is part of MS Keychain Gamer project. This is tiny game console.
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

/* This file describes hardware configuration, existing input and output peripherals */

#include <array>

#include <devices/x86/linux/usart.hpp>
#include <devices/x86/linux/i2c.hpp>
#include <devices/x86/linux/gpio.hpp>

namespace board
{

void board_init();

namespace gpio
{
    using RIGHT_KEY = hal::gpio::X86ButtonFactory<1>;
    using MID_KEY = hal::gpio::X86ButtonFactory<2>;
    using LEFT_KEY = hal::gpio::X86ButtonFactory<3>;
}

namespace interfaces
{
    std::array<hal::interfaces::Usart*, 1>& usarts();
}

}
