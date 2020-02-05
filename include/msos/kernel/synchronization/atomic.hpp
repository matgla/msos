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
namespace atomic
{

static inline void __dmb()
{
    asm volatile inline("dmb");
}

static inline int __strex(uint32_t value, volatile int* destination)
{
    volatile uint32_t output;
    asm volatile inline("strex %[result], %[val], [%[dest]]" : [result] "=&r"(output) : [dest] "r"(destination), [val] "r"(value) : "cc", "memory");
    return output;
}

static inline int __ldrex(volatile int* from)
{
    volatile uint32_t output;
    asm volatile inline("ldrex %[result], [%[source]]" : [result] "=&r"(output) : [source] "r"(from) : "cc", "memory");
    return output;
}


template <typename T>
static inline T increment(volatile T& source, const T value)
{
    asm volatile inline(
        "post_loop:\n\t"
        "ldrex r1, [%[from]]\n\t"
        "add r1, %[offset]\n\t"
        "strex r2, r1, [%[from]]\n\t"
        "cmp r2, #0\n\t"
        "bne post_loop\n\t"
        "dmb\n\t"
        :
        : [from] "r" (&source), [offset] "r" (value)
        );

    return true;
}

template <typename T>
static inline T decrement(volatile T& source, const T value)
{
    asm volatile inline("wait_loop:\n\t"
        "ldrex r1, [%[from]]\n\t"
        "sub r1, %[offset]\n\t"
        "strex r2, r1, [%[from]]\n\t"
        "cmp r2, #0\n\t"
        "bne wait_loop\n\t"
        "dmb\n\t"
        :
        : [from] "r" (&source), [offset] "r" (value));

    return true;
}

} // namespace atomic
} // namespace kernel
} // namespace msos

