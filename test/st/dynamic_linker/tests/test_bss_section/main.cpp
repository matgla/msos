#include "msos/utils/string.hpp"
#include <cstdint>

extern "C"
{
    void usart_write(const char* data);
}

static int data_arr[] = {1, 2, 3, 4};

static int data_1;
static int data_2 = 15;
static int data_3;

static int data_arr_2[] = {5, 6, 7, 8};

int main()
{
    usart_write("Hello from bss test\n");
    char buf[20];
    data_3 = 5;
    usart_write("Addresses of: \n");
    usart_write("data_arr: 0x");
    msos::utils::itoa(reinterpret_cast<uint32_t>(&data_arr[0]), buf, 16);
    usart_write(buf);
    usart_write("\n");
    usart_write("data_1: 0x");
    msos::utils::itoa(reinterpret_cast<uint32_t>(&data_1), buf, 16);
    usart_write(buf);
    usart_write("\n");

    usart_write("data_2: 0x");
    msos::utils::itoa(reinterpret_cast<uint32_t>(&data_2), buf, 16);
    usart_write(buf);
    usart_write("\n");

    usart_write("data_3: 0x");
    msos::utils::itoa(reinterpret_cast<uint32_t>(&data_3), buf, 16);
    usart_write(buf);
    usart_write("\n");

    usart_write("data_arr_2: 0x");
    msos::utils::itoa(reinterpret_cast<uint32_t>(&data_arr_2[0]), buf, 16);
    usart_write(buf);
    usart_write("\n");

    usart_write("Array 1: ");
    for (const auto data : data_arr)
    {
        msos::utils::itoa(data, buf);
        usart_write(buf);
        usart_write(", ");
    }
    usart_write("\n");

    usart_write("Integer 1: ");
    msos::utils::itoa(data_1, buf);
    usart_write(buf);
    usart_write("\n");

    usart_write("Integer 2: ");
    msos::utils::itoa(data_2, buf);
    usart_write(buf);
    usart_write("\n");

    usart_write("Integer 3: ");
    msos::utils::itoa(data_3, buf);
    usart_write(buf);
    usart_write("\n");

    usart_write("Array 2: ");
    data_arr_2[2] = 19;
    for (const auto data : data_arr_2)
    {
        msos::utils::itoa(data, buf);
        usart_write(buf);
        usart_write(", ");
    }
    usart_write("\n");

}


