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
#include <config.hpp>

#include <eul/error/error_code.hpp>
#include <eul/utils/math.hpp>

#include <msos/usart_printer.hpp>
#include <msos/libc/printf.hpp>

#include <msos/dynamic_linker/dynamic_linker.hpp>
#include <msos/dynamic_linker/environment.hpp>
#include <hal/core/backupRegisters.hpp>
#include <hal/core/core.hpp>
#include <symbol_codes.h>


static msos::dl::DynamicLinker dynamic_linker;
UsartWriter writer;

static uint32_t get_lot_at(uint32_t address)
{
    return dynamic_linker.get_lot_for_module_at(address);
}


int main()
{
    board::board_init();
    msos::system_config();
    const auto& usart = board::interfaces::usarts()[0];
    usart->init(9600);
    uint32_t address_of_lot_getter = reinterpret_cast<uint32_t>(&get_lot_at);
    uint32_t* lot_in_memory = reinterpret_cast<uint32_t*>(0x20000000);
    *lot_in_memory = address_of_lot_getter;

    std::size_t module_address = 0x08000000;
    module_address += 50 * 1024;
    msos::dl::Environment<2> env{
        msos::dl::SymbolAddress{SymbolCode::libc_atoi, &atoi},
        msos::dl::SymbolAddress{SymbolCode::libc_printf, &_printf}
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
    writer << "Execute binary" << endl;

    const char *args[2] {
        "110",
        "11"
    };

    int rc = module->execute(2, args);

    writer << "Returned " << dec << rc << endl;

    writer << "[TEST DONE]" << endl;

    while(true)
    {
    }
}
