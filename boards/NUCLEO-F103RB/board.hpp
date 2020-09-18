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
// left  button
// right button
// mid button

// i2c connectors

#include <devices/arm/stm32/stm32f1/stm32f103rbt6/gpio.hpp>
#include <devices/arm/stm32/stm32f1/stm32f103rbt6/usart.hpp>

namespace board
{

void board_init();

namespace gpio
{
using LED_GREEN = hal::gpio::PA5::OutputType;
using TEST = hal::gpio::PB12::OutputType;
}

namespace interfaces
{
using Usart1 = hal::devices::interfaces::Usart1;
}
}
