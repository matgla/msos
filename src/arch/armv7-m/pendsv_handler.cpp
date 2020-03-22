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

#include "msos/kernel/process/context_switch.hpp"

#include "arch/armv7-m/pendsv_handler.hpp"

namespace msos
{
namespace process
{

namespace
{

static bool first = true;

} // namespace


void initialize_pendsv()
{
    hal::interrupt::set_pendsv_handler([]{
        if (first)
        {
            first = false;
            switch_to_next_task();
            return;
        }

        store_and_switch_to_next_task();
    });
}

} // namespace process
} // namespace msos
