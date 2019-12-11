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

#include "msos/dynamic_linker/module.hpp"

#include "msos/usart_printer.hpp"

namespace msos
{
namespace dl
{

const std::string_view ModuleHeader::cookie() const
{
    return std::string_view(cookie_, 4);
}

const uint32_t ModuleHeader::code_size() const
{
    return code_size_;
}

const uint32_t ModuleHeader::data_size() const
{
    return data_size_;
}

const uint32_t ModuleHeader::bss_size() const
{
    return bss_size_;
}

const uint16_t ModuleHeader::number_of_relocations() const
{
    return number_of_relocations_;
}

const uint16_t ModuleHeader::total_relocations() const
{
    return total_relocations_;
}

const std::string_view ModuleHeader::name() const
{
    return std::string_view(reinterpret_cast<const char*>(this) + sizeof(ModuleHeader));
}

const uint32_t ModuleHeader::size() const
{
    return sizeof(ModuleHeader) + get_aligned_length(name());
}

} // namespace dl
} // namespace msos

