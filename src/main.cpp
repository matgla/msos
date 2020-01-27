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

#include <board.hpp>
#include <hal/time/sleep.hpp>
#include <hal/core/core.hpp>
//#include <hal/core/backupRegisters.hpp>
//#include <eul/utils/string.hpp>
//#include <hal/memory/heap.hpp>
//#include <string_view>

#include <hal/time/time.hpp>

#include "msos/usart_printer.hpp"
/*#include "msos/dynamic_linker/symbol.hpp"
#include "msos/dynamic_linker/relocation.hpp"
#include "msos/dynamic_linker/module.hpp"
#include "msos/dynamic_linker/environment.hpp"
#include "msos/dynamic_linker/dynamic_linker.hpp"
*/
#include "msos/kernel/process/process.hpp"
#include "msos/kernel/process/process_manager.hpp" 
#include "msos/kernel/process/scheduler.hpp"

#include "msos/kernel/synchronization/semaphore.hpp"
/*
#include <unistd.h>
*/
//msos::dl::DynamicLinker dynamic_linker;

UsartWriter writer;
/*uint32_t get_lot_at(uint32_t address)
{
    return dynamic_linker.get_lot_for_module_at(address);
}
*/
extern "C"
{
    void call_external(uint32_t address);
    void get_address();
    void test_function()
    {
        board::interfaces::Usart1::write("Test Function \n");
    }
    pid_t _fork();
}

/*
void write_to_usart(const char* data)
{
    board::interfaces::Usart1::write(data);
}

void test_main()
{
    write_to_usart("EEEEEHH\n");
}
*/
// void call_external(const uint32_t address)
// {
//    reinterpret_cast<void(*)()>(address)();
// }

/*const msos::dl::LoadedModule* load_module(msos::dl::DynamicLinker& linker, const uint32_t address)
{
    writer << "Loading module..., env symbol 0x" << hex << reinterpret_cast<uint32_t>(&write_to_usart) << endl;
    msos::dl::Environment<1> environment{
        msos::dl::SymbolAddress{"_Z14write_to_usartPKc", &write_to_usart}
    };

    return linker.load_module(address, msos::dl::LoadingModeCopyData, environment);
}
*/
//static int i = 1;

static msos::synchronization::Semaphore mutex(1);

void kernel_process()
{
    printf("Hello from Kernel\n");
    
    if (_fork())
    {
        writer << "Parent" << endl;
        
        mutex.wait();
       
        writer << "parent going to sleep" << endl;

        hal::time::sleep(std::chrono::seconds(5));

        writer << "Parent Done" << endl;
        mutex.post();
        while (true) {
            hal::time::sleep(std::chrono::milliseconds(100));
            writer << "Parent" << endl;
        }
    }
    else 
    {

        writer << "Child" << endl;
        mutex.wait(); 
        writer << "Child is going to sleep" << endl;
        hal::time::sleep(std::chrono::seconds(1));

        writer << "Child Done" << endl;
        mutex.post();
        while (true) {
            hal::time::sleep(std::chrono::milliseconds(500));
            writer << "Child" << endl;
        }
    }
}

int main()
{
    hal::core::Core::initializeClocks();
    using LED = board::gpio::LED_BLUE;
    LED::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    using Usart = board::interfaces::Usart1;
    Usart::init(9600);
    hal::time::Time::init();
    
    writer << "I am starting" << endl;
    hal::time::sleep(std::chrono::milliseconds(500));
    writer << "GO GO GO" << endl;
    root_process(reinterpret_cast<std::size_t>(&kernel_process));

    //    mutex.wait();
    while (true)
    {
    }

}

