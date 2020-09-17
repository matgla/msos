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

#include <hal/interfaces/usart.hpp>

#include <stm32f103c8t6_usart.hpp>
#include <stm32f103c8t6_i2c.hpp>
#include <stm32f103c8t6_gpio.hpp>

namespace board
{

void board_init();

namespace gpio
{

using VGA_RED_PIN_1 = hal::devices::gpio::PA0;
using VGA_RED_PIN_2 = hal::devices::gpio::PA1;

using VGA_GREEN_PIN_1 = hal::devices::gpio::PA4;
using VGA_GREEN_PIN_2 = hal::devices::gpio::PA5;

using VGA_BLUE_PIN_1 = hal::devices::gpio::PB8;
using VGA_BLUE_PIN_2 = hal::devices::gpio::PB9;

using VGA_HSYNC = hal::devices::gpio::PB10;
using VGA_VSYNC = hal::devices::gpio::PB11;

using LED = hal::devices::gpio::PB12;

}

namespace interfaces
{

std::array<hal::interfaces::Usart*, 1>& usarts();

}

}
