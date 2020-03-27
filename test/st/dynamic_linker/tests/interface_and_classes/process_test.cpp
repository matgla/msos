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

#include "process_test.hpp"

#include <cstdio>

#include "i_some_interface.hpp"
#include "implementation_a.hpp"
#include "implementation_b.hpp"
#include "child_implementation_a.hpp"

namespace test_processor
{

void test()
{
    ImplementationA a;
    ImplementationB b(15, "I_AM_B");
    ChildA a_c;

    ISomeInterface* i  = &a;

    // To be fixed with printf
    printf("ImplementationA value: %d\n", a.get_value());
    printf("ImplementationB value: %d\n", b.get_value());
    printf("ChildA value: %d\n", a_c.get_value());
    printf("Interface value: %d\n", i->get_value());
    printf("Printing tests :)\n");
    a.print();
    b.print();
    a_c.print();
    i->print();

    i = &a_c;
    printf("Interface value: %d\n", i->get_value());
    i->print();
    printf("Hello from module\n");
}

} // namespace test_processor
