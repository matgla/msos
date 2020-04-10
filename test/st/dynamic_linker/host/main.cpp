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

#include <unistd.h>
#include <board.hpp>
#include <cstring>

#include <eul/error/error_code.hpp>

#include <msos/usart_printer.hpp>
#include <msos/libc/printf.hpp>

#include <msos/dynamic_linker/dynamic_linker.hpp>
#include <msos/dynamic_linker/environment.hpp>
#include <hal/core/backupRegisters.hpp>
#include <hal/core/core.hpp>

static msos::dl::DynamicLinker dynamic_linker;
UsartWriter writer;

extern "C"
{
void usart_write(const char* data);

}

void usart_write(const char* data)
{
    writer << data;
}

static std::size_t get_lot_at(uint32_t address)
{
    writer << "Getting address of LOT: 0x" << hex << address << endl;
    return dynamic_linker.get_lot_for_module_at(address);
}


int main()
{
    board::board_init();
    hal::core::Core::initializeClocks();
    board::interfaces::Usart1::init(115200);

    std::size_t address_of_lot_getter = reinterpret_cast<std::size_t>(&get_lot_at);
    // hal::core::BackupRegisters::init();
    // hal::core::BackupRegisters::write(1, address_of_lot_getter >> 16);
    // hal::core::BackupRegisters::write(2, address_of_lot_getter);
    std::size_t* lot_in_memory = reinterpret_cast<std::size_t*>(0x20000000);
    *lot_in_memory = address_of_lot_getter;
    writer << "Address of lot getter: 0x" << hex << address_of_lot_getter << endl;
    writer << "Address of lot in memory: 0x" << hex << reinterpret_cast<std::size_t>(lot_in_memory) << endl;

    std::size_t module_address = 0x08000000;
    module_address += 32 * 1024;
    int extern_data = 123;
    msos::dl::Environment<4> env{
        msos::dl::SymbolAddress{300, &strlen},
        msos::dl::SymbolAddress{1100, &write},
        msos::dl::SymbolAddress{100, &_printf},
        msos::dl::SymbolAddress{102, &sprintf},
    };
    writer << "[TEST START]" << endl;

    eul::error::error_code ec;
    const msos::dl::LoadedModule* module = dynamic_linker.load_module(reinterpret_cast<std::size_t*>(module_address), msos::dl::LoadingModeCopyData, env, ec);

    if (module == nullptr)
    {
        writer << "Module not loaded properly" << endl;
        writer << "[TEST DONE]" << endl;
        while (true)
        {
        }
    }
    writer << "Module loaded" << endl;

    module->execute();

    writer << "[TEST DONE]" << endl;

    while(true)
    {
    }
}
