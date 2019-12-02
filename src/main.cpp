#include <board.hpp>
#include <hal/time/sleep.hpp>
#include <hal/core/core.hpp>
#include <hal/core/backupRegisters.hpp>
#include <eul/utils/string.hpp>
#include <hal/memory/heap.hpp>
#include <string_view>

#include "msos/usart_printer.hpp"
#include "msos/dynamic_linker/symbol.hpp"
#include "msos/dynamic_linker/relocation.hpp"
#include "msos/dynamic_linker/module.hpp"
#include "msos/dynamic_linker/environment.hpp"
#include "msos/dynamic_linker/dynamic_linker.hpp"

uint32_t lot_table_address = 0;

msos::dl::DynamicLinker dynamic_linker;

uint32_t get_lot_at(uint32_t address)
{
    return dynamic_linker.get_lot_for_module_at(address);
}

extern "C"
{
    void call_external(uint32_t address);
    void get_address();
    void test_function()
    {
        board::interfaces::Usart1::write("Test Function \n");
    }
}


void write_to_usart(const char* data)
{
    board::interfaces::Usart1::write(data);
}

void test_main()
{
    write_to_usart("EEEEEHH\n");
}

// void call_external(const uint32_t address)
// {
//    reinterpret_cast<void(*)()>(address)();
// }

const msos::dl::LoadedModule* load_module(msos::dl::DynamicLinker& linker, const uint32_t address)
{
    writer << "Loading module..., env symbol 0x" << hex << reinterpret_cast<uint32_t>(&write_to_usart) << endl;
    msos::dl::Environment<1> environment{
        msos::dl::SymbolAddress{"_Z14write_to_usartPKc", &write_to_usart}
    };

    return linker.load_module(address, msos::dl::LoadingModeCopyData, environment);
}

UsartWriter<board::interfaces::Usart1> writer;

int main()
{
    board::board_init();
    hal::core::Core::initializeClocks();
    using LED = board::gpio::LED_BLUE;
    LED::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    using Usart = board::interfaces::Usart1;
    Usart::init(9600);
    writer << "Sizeof Module: " << dec << sizeof(msos::dl::Module) << ", module data: " << sizeof(msos::dl::ModuleData) << ", loaded module: " << sizeof(msos::dl::LoadedModule) << endl;
    writer << "Hello from MSOS Kernel!" << endl;
    writer << "Heap usage: " << dec << hal::memory::get_heap_usage() << "/" << hal::memory::get_heap_size() << " bytes." << endl;
    uint32_t address = reinterpret_cast<uint32_t>(&get_lot_at);
    writer << "Address of function: 0x" << hex << address << endl;
    hal::core::BackupRegisters::init();
    hal::core::BackupRegisters::write(1, address >> 16);
    hal::core::BackupRegisters::write(2, address);

    std::size_t module_address = 0x08000000;
    module_address += 32 * 1024;

    const auto module = load_module(dynamic_linker, module_address);
    if (module == nullptr)
    {
        writer << "Module is nullptr" << endl;
        while (true)
        {
        }
    }
    writer << "Successfully loaded module: " << module->get_module().get_header().name() << endl;
    writer << "Heap usage: " << dec << hal::memory::get_heap_usage() << "/" << hal::memory::get_heap_size() << " bytes." << endl;
    module->execute();

    while (true)
    {
        LED::setHigh();
        hal::time::sleep(std::chrono::seconds(1));
        LED::setLow();
        hal::time::sleep(std::chrono::seconds(1));
        Usart::write("toggle\n");
    }
}

