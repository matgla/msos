#pragma once
// left  button
// right button
// mid button

// i2c connectors

#include <devices/stm/stm32/attiny85/gpio.hpp>

namespace board
{
namespace gpio
{
using BUTTON_LEFT = hal::devices::gpio::PA5::OutputType;
}
}