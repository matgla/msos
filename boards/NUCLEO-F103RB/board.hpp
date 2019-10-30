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
using LED_GREEN = hal::devices::gpio::PA5::OutputType;
using TEST = hal::devices::gpio::PB12::OutputType;
}

namespace interfaces 
{
using Usart1 = hal::devices::interfaces::Usart1;
}
}
