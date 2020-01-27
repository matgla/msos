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

#include "msos/kernel/synchronization/semaphore.hpp" 

namespace msos
{
namespace synchronization
{

Semaphore::Semaphore(uint32_t value)
    : value_(value)
{
}

static inline void __dmb()
{
    asm volatile inline("dmb");
}

static inline uint32_t __strex(uint32_t value, void* destination)
{
    uint32_t output;
    asm volatile inline("strex %[result], %[val], [%[dest]]" : [result] "=&r"(output) : [dest] "r"(destination), [val] "r"(value) : "cc", "memory"); 
    return output;
}

static inline uint32_t __ldrex(void* from)
{
    uint32_t output;
    asm volatile inline("ldrex %[result], [%[source]]" : [result] "=&r"(output) : [source] "r"(from) : "cc", "memory");
    return output;
}

int Semaphore::wait() 
{
   
    bool succeeded = false;
    
    int new_value =  __ldrex(&value_);
    
    while (--new_value < 0) 
    {
        new_value = __ldrex(&value_);
    };    

    while (!succeeded)
    {

        new_value = __ldrex(&value_);
   
        __dmb();
        if (0 == __strex(--new_value, &value_))
        {
            succeeded = true; 
        }
        __dmb();
    } 

    return true;
}

int Semaphore::post() 
{
    int new_value = 0;
    bool succeeded = false;
    while (!succeeded)
    {
        new_value = __ldrex(&value_);
        __dmb(); 
        if (0 == __strex(++new_value, &value_))
        {
            succeeded = true;
        }
        __dmb(); 
    }

    return true;
}

} // namespace synchronization 
} // namespace msos 

