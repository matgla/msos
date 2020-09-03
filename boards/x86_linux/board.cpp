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

#include "board.hpp"

extern "C"
{
    uint32_t _fs_flash_start = 0;
}

namespace board
{

namespace interfaces
{
std::array<hal::interfaces::Usart*, 1>& usarts()
{
    static hal::devices::interfaces::Usart1 usart1;
    static std::array<hal::interfaces::Usart*, 1> usarts_{&usart1};
    return usarts_;
}

}

void board_init()
{
}

}

