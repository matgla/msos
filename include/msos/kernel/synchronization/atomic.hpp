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

#pragma once

namespace msos
{
namespace kernel
{
static inline void __dmb()
{
    asm volatile inline("dmb");
}

namespace atomic
{

static inline uint32_t __strex(uint32_t value, const void* destination)
{
    uint32_t output;
    asm volatile inline("strex %[result], %[val], [%[dest]]" : [result] "=&r"(output) : [dest] "r"(destination), [val] "r"(value) : "cc", "memory");
    return output;
}

static inline uint32_t __ldrex(const void* from)
{
    uint32_t output;
    asm volatile inline("ldrex %[result], [%[source]]" : [result] "=&r"(output) : [source] "r"(from) : "cc", "memory");
    return output;
}


template <typename T>
T increment(const T& source, const T value)
{
    T new_value;
    uint32_t success = 1;
    do
    {
        __dmb();
        new_value = __ldrex(&source) + value;
        __dmb();
        success = __strex(new_value, &source);
        __dmb();
    } while (success);
    __dmb();
    return new_value;
}

template <typename T>
T decrement(const T& source, const T value)
{
    T new_value;
    uint32_t success = 1;
    do
    {
        __dmb();
        new_value = __ldrex(&source) - value;
        __dmb();
        success = __strex(new_value, &source);
        __dmb();
    } while (success);
    __dmb();
    return new_value;
}

} // namespace atomic
} // namespace kernel
} // namespace msos

