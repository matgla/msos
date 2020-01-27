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
#include "msos/kernel/synchronization/atomic.hpp"

namespace msos
{
namespace kernel 
{
namespace synchronization
{

Semaphore::Semaphore(uint32_t value)
    : value_(value)
{
}

int Semaphore::wait() 
{
    int new_value =  atomic::__ldrex(&value_);
    
    while (--new_value < 0) 
    {
        new_value = atomic::__ldrex(&value_);
    };    

    atomic::decrement(value_, 1);

    return true;
}

int Semaphore::post() 
{
    atomic::increment(value_, 1); 

    return true;
}

} // namespace synchronization 
} // namespace kernel
} // namespace msos 

