#include <board.hpp>
#include <hal/time/sleep.hpp>
#include <hal/core/core.hpp>

int main()
{
    board::board_init();   
    hal::core::Core::initializeClocks();
    using LED = board::gpio::LED_BLUE;
    LED::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    using Usart = board::interfaces::Usart1; 
    Usart::init(9600);
    Usart::write("Hello from MSOS Kernel!\n");
    while (true)
    {
        LED::setHigh();
        hal::time::sleep(std::chrono::seconds(1));
        LED::setLow();
        hal::time::sleep(std::chrono::seconds(1));
        Usart::write("toggle\n");
    }
}

