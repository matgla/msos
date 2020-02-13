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

extern "C"
{

static __inline__ uint32_t get_program_counter()
{
    uint32_t pc;
    asm ("mov %0, pc" : "=r"(pc));
    return pc;
}

static __inline__ uint32_t get_sp()
{
    uint32_t sp;
    asm ("mrs %0, psp" : "=r"(sp));
    return sp;
}

static __inline__ uint32_t get_lr()
{
    uint32_t lr;
    asm ("mov %0, lr" : "=r"(lr));
    return lr;
}

void context_switch(std::size_t stack);
std::size_t* get_psp();
void load_context(uint32_t sp);
void load_context_msp(uint32_t sp);
void store_context();
void store_context_msp();

}

