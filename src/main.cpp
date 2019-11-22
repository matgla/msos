#include <board.hpp>
#include <hal/time/sleep.hpp>
#include <hal/core/core.hpp>
#include <hal/core/backupRegisters.hpp>
#include <eul/utils/string.hpp>
#include <string_view>

#include "msos/usart_printer.hpp"
#include "msos/dynamic_linker/symbol.hpp"
#include "msos/dynamic_linker/relocation.hpp"
#include "msos/dynamic_linker/module.hpp"

void call_me()
{
    using Usart = board::interfaces::Usart1;
    Usart::write("I was called :)\n");
}

extern "C"
{
    int get_address();
}



void write_to_usart(const char* data)
{
    board::interfaces::Usart1::write(data);
}

template <typename Writer>
void process_module(Writer& writer, uint32_t address, uint8_t* memory_buffer)
{
    writer << "Started processing of module at address: 0x" << hex << address << endl;
    const msos::dl::Module* module = reinterpret_cast<const msos::dl::Module*>(address);
    writer << "Magic cookie: " << module->cookie() << endl;
    writer << "size:" << dec << endl
           << "    code: " << module->code_size() << endl
           << "  rodata: " << module->rodata_size() << endl
           << "    data: " << module->data_size() << endl
           << "     bss: " << module->bss_size() << endl;

    writer << "number of relocations: " << module->number_of_relocations() << endl;
    writer << "total relocations: " << module->total_relocations() << endl;

    writer << "Module name: " << module->name() << endl;
    address += module->size();

    for (int i = 0; i < module->number_of_relocations(); ++i)
    {
        const msos::dl::Relocation* relocation = reinterpret_cast<const msos::dl::Relocation*>(address);
        address += relocation->size();
        writer << "Relocation (relocation index, symbol index): " << relocation->index() << ", " << relocation->symbol()->name() << endl;
    }

    uint32_t number_of_symbols = *reinterpret_cast<uint32_t*>(address);
    address += 4;

    writer << "Number of symbols: " << number_of_symbols << endl;

    for (uint32_t i = 0; i < number_of_symbols; ++i)
    {
        msos::dl::Symbol* sym = reinterpret_cast<msos::dl::Symbol*>(address);
        address += sym->size();
        writer << sym->name() << " : size: "
               << sym->size() << ", visibility: "
               << msos::dl::to_string(sym->visibility()) << ", section: "
               << msos::dl::to_string(sym->section()) << ", symbol: "
               << endl;
    }

    address = address % 4 ? address + (4 - (address % 4)) : address;

    uint32_t code_address = address;
    address += module->code_size();
    uint32_t rodata_address = address;
    address += module->rodata_size();
    uint32_t data_address = address;

    memory_buffer[0] = reinterpret_cast<uint32_t>(&memory_buffer[9]);
    memory_buffer[4] = reinterpret_cast<uint32_t>(&write_to_usart);
    std::memcpy(&memory_buffer[9], reinterpret_cast<const uint8_t*>(data_address), module->data_size());


    writer << "Section addresses in primary memory:" << endl
        << "    code: 0x" << hex << code_address << endl
        << "  rodata: 0x" << hex << rodata_address << endl
        << "    data: 0x" << hex << data_address << endl;

}

int main()
{

    board::board_init();
    hal::core::Core::initializeClocks();
    using LED = board::gpio::LED_BLUE;
    LED::init(hal::gpio::Output::OutputPushPull, hal::gpio::Speed::Default);
    using Usart = board::interfaces::Usart1;
    Usart::init(9600);

    UsartWriter<Usart> writer;
    writer << "Hello from MSOS Kernel!" << endl;
    uint32_t address = reinterpret_cast<uint32_t>(&call_me);
    writer << "Address of function: 0x" << hex << address << endl;
    hal::core::BackupRegisters::init();
    hal::core::BackupRegisters::write(1, address >> 16);
    hal::core::BackupRegisters::write(2, address);

    uint32_t readed_address = get_address();
    writer << "Readed addres: 0x" << hex << readed_address << endl;
    void(*call)()  = reinterpret_cast<void(*)()>(readed_address);
    hal::core::BackupRegisters::reset();
    readed_address = get_address();
    writer << "Readed addres: 0x" << hex << readed_address << endl;
    writer << "test:" << 0xabcdef12 << endl;
    std::size_t module_address = 0x08000000;
    module_address += 32 * 1024;

    uint8_t memory_buffer_[1024];
    process_module(writer, module_address, memory_buffer_);


    while (true)
    {
        call();
        LED::setHigh();
        hal::time::sleep(std::chrono::seconds(1));
        LED::setLow();
        hal::time::sleep(std::chrono::seconds(1));
        Usart::write("toggle\n");
    }
}

