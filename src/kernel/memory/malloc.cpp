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

#include <cstdlib>

#include "msos/usart_printer.hpp"

namespace
{
UsartWriter writer;
}

extern "C"
{

void* __real__malloc_r (struct _reent *r, size_t sz);

void* __wrap__malloc_r (struct _reent *r, size_t sz)
{
    void* mem = __real__malloc_r(r, sz);
    writer << "Allocated " << dec << sz << " bytes at: 0x" << hex << reinterpret_cast<std::size_t>(mem) << endl;
    return mem;
}// This file is part of MSOS project. This is simple OS for embedded development devices.
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

void* __real__free_r (struct _reent *r, void* data);

void* __wrap__free_r (struct _reent *r, void* data)
{
    writer << "Free called at: 0x" << hex << reinterpret_cast<std::size_t>(data) << endl;
    return __real__free_r(r, data);
}

void* __real__realloc_r (struct _reent *r, void* addr, size_t sz);

void* __wrap__realloc_r (struct _reent *r, void* addr, size_t sz)
{
    writer << "Realloc called with size: " << sz << endl;
    return __real__realloc_r(r, addr, sz);
}

}
