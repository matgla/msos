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

#include <msos/usart_printer.hpp>

#include <msos/dynamic_linker/dynamic_linker.hpp>
#include <msos/dynamic_linker/environment.hpp>
#include <hal/core/backupRegisters.hpp>
#include <hal/core/core.hpp>

msos::dl::DynamicLinker dynamic_linker;
UsartWriter writer;

extern "C"
{
void usart_write(const char* data);

}

void usart_write(const char* data)
{
    writer << data;
}

uint32_t get_lot_at(uint32_t address)
{
    writer << "Getting address of LOT: 0x" << hex << address << endl;
    return dynamic_linker.get_lot_for_module_at(address);
}


int main()
{
    board::board_init();
    hal::core::Core::initializeClocks();
    board::interfaces::Usart1::init(9600);

    uint32_t address_of_lot_getter = reinterpret_cast<uint32_t>(&get_lot_at);
    hal::core::BackupRegisters::init();
    hal::core::BackupRegisters::write(1, address_of_lot_getter >> 16);
    hal::core::BackupRegisters::write(2, address_of_lot_getter);
    writer << "Address of lot getter: 0x" << hex << address_of_lot_getter << endl;

    std::size_t module_address = 0x08000000;
    module_address += 32 * 1024;
    int extern_data = 123;
    msos::dl::Environment<4> env{
        msos::dl::SymbolAddress{"usart_write", &usart_write},
        msos::dl::SymbolAddress{"strlen", &strlen},
        msos::dl::SymbolAddress{"write", &write},
        msos::dl::SymbolAddress{"extern_1", &extern_data}
    };
    writer << "[TEST START]" << endl;

    const msos::dl::LoadedModule* module = dynamic_linker.load_module(module_address, msos::dl::LoadingModeCopyData, env);

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
