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
#include "msos/kernel/synchronization/mutex.hpp"

#include "msos/drivers/storage/ram_block_device.hpp"
/*
#include <unistd.h>
*/
//msos::dl::DynamicLinker dynamic_linker;

hal::UsartWriter writer;
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
    std::size_t* get_psp();
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

static msos::kernel::synchronization::Semaphore mutex(1);

msos::kernel::synchronization::Mutex mutex_;

void printa()
{
    writer << "Some print" << endl;
}

void b_finish()
{
    writer << "B finished" << endl;
}

void print_stack(std::size_t length)
{
    const uint32_t* stack;
    asm inline("mrs %0, psp" : "=r"(stack));
    stack-=4;
    for (std::size_t i = 0; i < length; i+=4)
    {
        printf("%p: 0x%08x 0x%08x 0x%08x 0x%08x\n", &stack[i], stack[i], stack[i+1], stack[i+2], stack[i+3]);
    }
}

void child_fun()
{
    writer << "Child" << endl;
    mutex_.lock();
    writer << "Child is going to sleep" << endl;
    hal::time::sleep(std::chrono::milliseconds(500));

    writer << "Child Done" << endl;
    printa();
    mutex_.unlock();
    int i = 0;
    int data = 0xfacefade;
    int out;
    // asm volatile inline (
    //             "mov %[out], %[in]\n\t"
    //             "push {%[out]}\n\t"
    //             : [out] "=r" (out)
    //             : [in] "r" (data)
    //             );
    if (_fork())
    {
        printf("Stack in child A\n");
        print_stack(10);
        while (i < 2)
        {
            hal::time::sleep(std::chrono::milliseconds(500));
            writer << "Child A" << endl;
            i++;
        }

        writer << "Child A finished" << endl;
    }
    else
    {
        printf("Stack in child B, current sp: %x\n", get_psp());
        out = 0;
        // asm volatile inline (
        //     "pop {%[out]}\n\t"
        //     : [out] "=r" (out)
        // );
        // printf("Pooped value: %x\n");
        // asm volatile inline (
        //     "pop {%[out]}\n\t"
        //     : [out] "=r" (out)
        // );
        // printf("Pooped value: %x\n");
        // asm volatile inline (
        //     "pop {%[out]}\n\t"
        //     : [out] "=r" (out)
        // );
        // printf("Pooped value: %x\n");
        // asm volatile inline (
        //     "pop {%[out]}\n\t"
        //     : [out] "=r" (out)
        // );
        // printf("Pooped value: %x\n");
        // asm volatile inline (
        //     "pop {%[out]}\n\t"
        //     : [out] "=r" (out)
        // );
        // printf("Pooped value: %x\n");
        print_stack(10);

        while (i < 4)
        {
            hal::time::sleep(std::chrono::milliseconds(500));
            writer << "Child B" << endl;
            i++;
        }

        b_finish();
    }

    return;
}

void kernel_process()
{
    printf("Hello from Kernel\n");

    if (_fork())
    {
        writer << "Parent" << endl;

        mutex_.lock();

        writer << "parent going to sleep" << endl;

        hal::time::sleep(std::chrono::seconds(1));

        writer << "Parent Done" << endl;
        mutex_.unlock();
        int i = 0;
        while (true) {
            hal::time::sleep(std::chrono::milliseconds(100));
            writer << "Parent: " << i << endl;
            ++i;
            if (i == 10)
            {
                writer << "Forking" << endl;
                if (_fork())
                {
                    writer << "Old parent" << endl;
                }
                else
                {
                    int x = 0;
                    writer << "Child C is starting" << endl;
                    break;
                    while (x < 10)
                    {
                        // writer << "Child C is working " << x << endl;
                        hal::time::sleep(std::chrono::milliseconds(50));
                        x++;
                    }
                }
            }
        }
    }
    else
    {
        child_fun();
    }

    printf("Some child died\n");
    // Child process default exit
    exit(0);

    // Main process should never goes here
    while (true) {}
}
#include <cstring>
int main()
{
    hal::core::Core::initializeClocks();
    using LED = board::gpio::LED_BLUE;
    LED::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    using Usart = board::interfaces::Usart1;
    Usart::init(9600);
    hal::time::Time::init();

    writer << "I am starting" << endl;

    msos::drivers::storage::RamBlockDevice bd(2048, 1, 1, 1);

    int error = bd.init();
    if (error)
    {
        writer << "Cannot initialize block device" << endl;
    }

    writer << "Device geometry: " << endl
        << "Size: " << bd.size() << endl
        << "Read size: " << bd.read_size() << endl
        << "Write size: " << bd.write_size() << endl
        << "Erase size: " << bd.erase_size() << endl;

    char buffer[24];
    gsl::span<uint8_t> buffer_span = gsl::make_span(reinterpret_cast<uint8_t*>(buffer), 24);

    std::strncpy(buffer, "Hello", sizeof(buffer));

    writer << "Erase 24 bytes at 0x0" << endl;
    error = bd.erase(0x0, 24);
    if (error)
    {
        writer << "Can't erase" << endl;
    }

    writer << "Write Hello at address 0x4" << endl;
    error = bd.write(0x04, buffer_span);

    if (error)
    {
        writer << "Can't write to bd" << endl;
    }

    std::memset(buffer, 0, 24);

    writer << "Read" << endl;
    error = bd.read(0x0, buffer_span);
    if (error)
    {
        writer << "Can't read from bd" << endl;
    }

    for (char byte : buffer)
    {
        writer << hex << static_cast<int>(byte) << " : ";
        writer << byte << endl;
    }


    //hal::time::sleep(std::chrono::milliseconds(500));

    // __disable_irq();
    root_process(reinterpret_cast<std::size_t>(&kernel_process));

    while (true)
    {
    }

}

