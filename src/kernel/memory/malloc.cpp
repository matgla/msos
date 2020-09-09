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
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include <cstdlib>

#include "msos/usart_printer.hpp"

namespace
{
UsartWriter writer;
}

extern "C"
{

static int current_usage = 0;
extern uint32_t _ram_start;
extern uint32_t _ram_end;
static int max_usage = (&_ram_end - &_ram_start)*4;

void* __real_malloc (size_t sz);

void* __wrap_malloc (size_t sz)
{
    size_t* mem = static_cast<size_t*>(__real_malloc(sz + sizeof(size_t)));
    mem[0] = sz + sizeof(size_t);
    current_usage += sz + sizeof(size_t);
    // writer << "malloc:  Usage: [" << current_usage << "/" << max_usage << "] bytes"
        // << ", added: " << (sz + sizeof(size_t))
        // << endl;
    return &mem[1];
}


void* __real_free (void* data);

void* __wrap_free (void* data)
{
    size_t* memory = &static_cast<size_t*>(data)[-1];
    current_usage -= memory[0];
    // writer << "free:    Usage: [" << current_usage << "/" << max_usage << "] bytes"
        // << ", released: " << memory[0]
        // << endl;
    return __real_free(static_cast<void*>(memory));
}

void* __real_realloc (void* addr, size_t sz);

void* __wrap_realloc (void* addr, size_t sz)
{
    size_t* memory = &static_cast<size_t*>(addr)[-1];
    current_usage -= memory[0];
    size_t* mem = static_cast<size_t*>(__real_realloc(addr, sz + sizeof(size_t)));
    current_usage += sz;
    // writer << "realloc: Usage: [" << current_usage << "/" << max_usage << "] bytes" << endl;

    return &mem[1];
}

}
