#include <board.hpp>

int main()
{
    board::gpio::LED_GREEN::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    board::gpio::LED_GREEN::setHigh();
}
