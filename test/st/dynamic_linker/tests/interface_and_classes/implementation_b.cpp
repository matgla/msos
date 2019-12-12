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

#include "implementation_b.hpp"

#include <msos/usart_printer.hpp>

ImplementationB::ImplementationB(int value, const std::string_view& name)
    : value_(value)
    , name_(name)
{
}


ImplementationB::~ImplementationB()
{
    UsartWriter writer;
    writer << "~ImplementationB()" << endl;
}

void ImplementationB::print()
{
    UsartWriter writer;
    writer << name_ << endl;
}

int ImplementationB::get_value()
{
    return value_;
}

