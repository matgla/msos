// This file is part of MSOS project. This is simple OS for embedded development devices.
// Copyright (C) 2019 Mateusz Stadnik
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <type_traits>

#include <cstring>
#include <cstdint>

#include "msos/utils/string.hpp"

extern "C"
{
    void usart_write(const char* data);
}

int global_integer = 177176;

void print()
{
    static uint32_t data[] = {1, 2, 3, 4};
    usart_write("Local data 1: ");
    int data_2[] = {5, 6, 7, 8};
    data[3] = 8;
    char buf[20];
    for (auto d : data)
    {
        msos::utils::itoa(d, buf);
        usart_write(buf);
        usart_write(", ");
    }
    usart_write("\n");

    usart_write("Local data 2: ");
    data[0] = -12;
    for (auto d : data_2)
    {
        msos::utils::itoa(d, buf);
        usart_write(buf);
        usart_write(", ");
    }
    usart_write("\n");
}

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

    print();
}

