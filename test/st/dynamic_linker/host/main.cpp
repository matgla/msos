#include <board.hpp>

#include <msos/usart_printer.hpp>

#include <msos/dynamic_linker/dynamic_linker.hpp>
#include <msos/dynamic_linker/environment.hpp>
#include <hal/core/backupRegisters.hpp>
#include <hal/core/core.hpp>
msos::dl::DynamicLinker dynamic_linker;

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

UsartWriter<board::interfaces::Usart1> writer;

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

    msos::dl::Environment<1> env{
        msos::dl::SymbolAddress{"usart_write", &usart_write}
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

    module->execute(); 

    writer << "[TEST DONE]" << endl;

    return -1;
}
