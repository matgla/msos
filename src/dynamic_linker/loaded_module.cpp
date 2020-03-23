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

#include "msos/dynamic_linker/loaded_module.hpp"

#include <utility>

extern "C"
{
int call_external(uint32_t address);
}

namespace msos
{
namespace dl
{

LoadedModule::LoadedModule()
{
}

LoadedModule::LoadedModule(const ModuleHeader& header)
{
    module_.emplace(header);
}

const Module& LoadedModule::get_module() const
{
    return *module_;
}

Module& LoadedModule::get_module()
{
    return *module_;
}

void LoadedModule::set_start_address(const std::size_t start_address)
{
    start_address_ = start_address;
}

int LoadedModule::execute(int argc, char *argv[]) const
{
    return -1;
}

int LoadedModule::execute() const
{
    printf("Start address 0x%x\n", start_address_);
    return call_external(start_address_);
}

} // namespace dl
} // namespace msos

