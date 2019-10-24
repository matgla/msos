#pragma once
// left  button
// right button
// mid button

// i2c connectors

#include <devices/arm/stm32/stm32f1/stm32f103rbt6/gpio.hpp>

namespace board
{
namespace gpio
{
using LED_GREEN = hal::devices::gpio::PA5::OutputType;
}
}