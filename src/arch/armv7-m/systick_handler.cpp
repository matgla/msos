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

#include <chrono>

#include <hal/interrupt/pendsv.hpp>
#include <hal/interrupt/systick.hpp>

#include "msos/kernel/process/context_switch.hpp"

#include "arch/armv7-m/pendsv_handler.hpp"

namespace msos
{
namespace process
{

static std::chrono::milliseconds context_switch_period(20);

void initialize_systick()
{
    hal::interrupt::set_systick_handler([](std::chrono::milliseconds time)
    {
        static std::chrono::milliseconds last_time = time;

        if (std::chrono::duration_cast<std::chrono::milliseconds>(time - last_time) >= std::chrono::milliseconds(context_switch_period))
        {
            hal::interrupt::trigger_pendsv();

            last_time = time;
        }
    });
}

void change_context_switch_period(std::chrono::milliseconds period)
{
    context_switch_period = period;
}

} // namespace process
} // namespace msos
