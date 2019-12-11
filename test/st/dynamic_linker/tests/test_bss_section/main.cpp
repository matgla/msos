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

#include "msos/utils/string.hpp"
#include <cstdint>

extern "C"
{
    void usart_write(const char* data);
}

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

    usart_write("local_data: 0x");
    msos::utils::itoa(reinterpret_cast<uint32_t>(&data[0]), buf, 16);
    usart_write(buf);
    usart_write("\n");

    usart_write("local_data_2: 0x");
    msos::utils::itoa(reinterpret_cast<uint32_t>(&data_2[0]), buf, 16);
    usart_write(buf);
    usart_write("\n");
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

    print();

}


