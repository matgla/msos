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

#include <cstdio>
#include <cstdint>

void print()
{
    static uint32_t data[] = {1, 2, 3, 4};
    printf("Local data 1: ");
    int data_2[] = {5, 6, 7, 8};
    data[3] = 8;
    char buf[20];
    for (auto d : data)
    {
        sprintf(buf, "%d", d);
        printf("%s, ", buf);
    }
    printf("\n");

    printf("Local data 2: ");
    data[0] = -12;
    for (auto d : data_2)
    {
        sprintf(buf, "%d", d);
        printf("%s, ", buf);
    }
    printf("\n");

    printf("local_data: 0x");
    sprintf(buf, "%x", reinterpret_cast<uint32_t>(&data[0]));

    printf("%s\n", buf);

    printf("local_data_2: 0x");
    sprintf(buf, "%x", reinterpret_cast<uint32_t>(&data_2[0]));

    printf("%s\n", buf);
}


static int data_arr[] = {1, 2, 3, 4};

static int data_1;
static int data_2 = 15;
static int data_3;

static int data_arr_2[] = {5, 6, 7, 8};

int main()
{
    printf("Hello from bss test\n");
    char buf[20];
    data_3 = 5;
    printf("Addresses of: \n");
    printf("data_arr: 0x");
    sprintf(buf, "%x", reinterpret_cast<uint32_t>(&data_arr[0]));

    printf("%s\n", buf);
    printf("data_1: 0x");
    sprintf(buf, "%x", reinterpret_cast<uint32_t>(&data_1));
    printf("%s\n", buf);

    printf("data_2: 0x");
    sprintf(buf, "%x", reinterpret_cast<uint32_t>(&data_2));
    printf("%s\n", buf);

    printf("data_3: 0x");
    sprintf(buf, "%x", reinterpret_cast<uint32_t>(&data_3));
    printf("%s\n", buf);

    printf("data_arr_2: 0x");
    sprintf(buf, "%x", reinterpret_cast<uint32_t>(&data_arr_2[0]));
    printf("%s\n", buf);

    printf("Array 1: ");
    for (const auto data : data_arr)
    {
        sprintf(buf, "%d", data);
        printf("%s, ", buf);
    }
    printf("\n");

    printf("Integer 1: ");
    sprintf(buf, "%d", data_1);
    printf("%s\n", buf);

    printf("Integer 2: ");
    sprintf(buf, "%d", data_2);
    printf("%s\n", buf);

    printf("Integer 3: ");
    sprintf(buf, "%d", data_3);
    printf("%s\n", buf);

    printf("Array 2: ");
    data_arr_2[2] = 19;
    for (const auto data : data_arr_2)
    {
        sprintf(buf, "%d", data);
        printf("%s, ", buf);
    }
    printf("\n");
    print();
}


