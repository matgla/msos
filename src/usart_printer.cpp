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

#include "msos/usart_printer.hpp"

#include <board.hpp>

logging_flags::logging_flags()
    : base_(base::dec)
    , boolalpha_(boolalpha::disabled)
{
}

void logging_flags::set_base(const base new_base)
{
    base_ = new_base;
}

logging_flags::base logging_flags::get_base() const
{
    return base_;
}

void logging_flags::set_boolalpha(const boolalpha new_boolalpha)
{
    boolalpha_ = new_boolalpha;
}

logging_flags::boolalpha logging_flags::get_boolalpha() const
{
    return boolalpha_;
}


UsartWriter& UsartWriter::operator<<(const EndlineTag)
{
    write("\n");
    return *this;
}

UsartWriter& UsartWriter::operator<<(const logging_flags::base base)
{
    flags_.set_base(base);
    return *this;
}

void UsartWriter::write(const std::string_view& str)
{
    static_cast<void>(str);
    board::USART_1::write(str);
}


void UsartWriter::write(char data)
{
    char buf[] = { data, '\0' };
    write(buf);
}

int UsartWriter::get_base() const
{
    switch (flags_.get_base())
    {
        case logging_flags::base::dec:
        {
            return 10;
        } break;
        case logging_flags::base::hex:
        {
            return 16;
        } break;
        case logging_flags::base::oct:
        {
            return 8;
        } break;
        case logging_flags::base::bin:
        {
            return 2;
        } break;
    }
    return 0;
}

