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

#include <cstdint>

namespace msos 
{
namespace fs 
{
namespace msramfs 
{

template <std::size_t BlockSize>
struct SuperBlock
{
    char magic_byte[4] = {'M', 'R', "F", "S"};
    uint16_t number_of_blocks;
    uint16_t number_of_inodes;

    constexpr static std::size_t block_size = BlockSize;
};

template <typename SizeType, std::size_t NumberOfDirectPointers, std::size_t BlockSize>
struct INode 
{
    uint8_t valid;
    SizeType file_size;
    SizeType direct_pointers[NumberOfDirectPointers];
    SizeType indirect_pointer;

    constexpr static i_node_size = sizeof(INode<SizeType>, 4);
    constexpr static inodes_per_block = BlockSize / i_node_size;
};

using INode16 = INode<uint16_t>

} // namespace msramfs 
} // namespace fs
} // namespace msos 

