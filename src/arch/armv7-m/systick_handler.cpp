// This file is part of MSOS project.
// Copyright (C) 2020 Mateusz Stadnik
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

#include <hal/interrupt/pendsv.hpp>
#include <hal/interrupt/systick.hpp>

#include "arch/armv7-m/pendsv_handler.hpp"

namespace msos
{
namespace process
{

void initialize_systick()
{
    hal::interrupt::set_systick_handler([](std::chrono::milliseconds time)
    {
        static std::chrono::milliseconds last_time = time;

        if (std::chrono::duration_cast<std::chrono::milliseconds>(time - last_time) >= std::chrono::milliseconds(100))
        {
            hal::interrupt::trigger_pendsv();

            last_time = time;
        }
    });
}

} // namespace process
} // namespace msos
