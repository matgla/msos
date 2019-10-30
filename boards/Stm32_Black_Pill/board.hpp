#pragma once 

#include <devices/arm/stm32/stm32f1/stm32f103c8t6/gpio.hpp>
#include <devices/arm/stm32/stm32f1/stm32f103c8t6/usart.hpp>

namespace board 
{

void board_init();

namespace gpio 
{
using LED_BLUE = hal::devices::gpio::PB12::OutputType;
}

namespace interfaces 
{
using Usart1 = hal::devices::interfaces::Usart1;
}

}
