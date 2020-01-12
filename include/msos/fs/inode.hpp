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
namespace fs 
{

/* 
 * 3 - bits = 0 - 7
 * 5 - bits = 0 - 31
 *
 */

struct INode 
{
    enum class NodeType : uint8_t 
    {
        File = 0,
        SymbolicLink = 1,
        Directory = 2,
        CharDevice = 3,
        BlockDevice = 4,
        Socket = 5, 
        FIFO = 6
    };
    
    NodeType type : 3;
    uint8_t user_id : 5;
    uint8_t 
    uint32_t modification_time;
    uint32_t size;
};

} // namespace fs
} // namespace msos

