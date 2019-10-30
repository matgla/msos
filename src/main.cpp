#include <board.hpp>
#include <hal/time/sleep.hpp>
#include <hal/core/core.hpp>

int main()
{
    board::board_init();   
    hal::core::Core::initializeClocks();
    board::gpio::LED_GREEN::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    while (true)
    {
        board::gpio::LED_GREEN::setHigh();
        hal::time::sleep(std::chrono::seconds(1));
        board::gpio::LED_GREEN::setLow();
        hal::time::sleep(std::chrono::seconds(1));
    }
}

