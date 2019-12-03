#include <board.hpp>

#include <msos/usart_printer.hpp>

int main()
{
    board::board_init();
    board::interfaces::Usart1::init(9600);

    writer << "[TEST START]" << endl;

    writer << "[TEST DONE]" << endl;

    return -1;
}
