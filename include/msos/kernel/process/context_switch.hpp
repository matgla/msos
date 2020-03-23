// This file is part of MSOS project.
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

#include <cstdint>

#include <chrono>

extern "C"
{

void update_stack_pointer(const std::size_t* stack);
const std::size_t* get_next_task();
void switch_to_next_task();
void store_and_switch_to_next_task();

}

namespace msos
{
namespace process
{

void initialize_context_switching();
void change_context_switch_period(std::chrono::milliseconds period);


} // namespace process
} // namespace msos


