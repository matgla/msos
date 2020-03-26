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

#include <cstdio>
#include <cstring>
#include <cstdint>

#include "msos/utils/string.hpp"

int global_integer = 177176;

template <typename T, std::size_t N>
void print_array(const T(&t)[N])
{
    // UsartWriter writer;

    for (std::size_t i = 0; i < N - 1; ++i)
    {
    //    writer << t[i] << ", ";
    }
    // writer << t[N - 1] << endl;
}

void print()
{
    static uint32_t data[] = {1, 2, 3, 4};
    int data_2[] = {5, 6, 7, 8};
    data[3] = 8;
    // writer << "Local data 1: ";
    print_array(data);

    data[0] = -12;

    // writer << "Local data 2: ";
    print_array(data_2);
}


extern int extern_1;
extern int extern_2;

int main()
{
    global_integer = 177177;
    static const char* local_string = "Module started";
    // writer << local_string << endl;

    int local_int = -123456;
    constexpr int const_int = 21234;
    const char* integer_text = "Integer: ";
    // writer << integer_text << local_int << endl;
    // writer << "Constexpr integer: " << const_int << endl;
    // writer << "Global integer: " << global_integer << endl;

    print();

    int *ptr = &global_integer;

    *ptr = 19;
    // writer << "Global integer: " << global_integer << endl;
    // writer << "Global integer ptr: " << *ptr << endl;
    // writer << "Extern 1: " << extern_1 << endl;
    // writer << "Extern 2: " << extern_2 << endl;
    int* p_extern1 = &extern_1;
    *p_extern1 = extern_2;


    // writer << "Extern 1: " << extern_1 << endl;
    // writer << "Extern 2: " << extern_2 << endl;
}

