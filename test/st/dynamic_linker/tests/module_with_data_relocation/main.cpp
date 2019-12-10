#include <type_traits>

#include <cstring>

#include "msos/utils/string.hpp"

extern "C"
{
    void usart_write(const char* data);
}


int global_integer = 177176;

int main()
{
    global_integer = 177177;
    static const char* local_string = "Module started\n";
    usart_write(local_string);

    int local_int = -123456;
    constexpr int const_int = 21234;
    char data_buffer[30];
    msos::utils::itoa(local_int, data_buffer);
    static const char* integer_text = "Integer: ";
    usart_write(integer_text);
    usart_write(data_buffer);
    usart_write("\n");
    static const char* constexpr_text = "Constexpr integer: ";
    usart_write(constexpr_text);
    msos::utils::itoa(const_int, data_buffer);
    usart_write(data_buffer);
    usart_write("\n");
    static const char* global_text = "Global integer: ";
    usart_write(global_text);
    msos::utils::itoa(global_integer, data_buffer);
    usart_write(data_buffer);
    usart_write("\n");

}

