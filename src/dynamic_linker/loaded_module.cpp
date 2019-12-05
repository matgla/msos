#include "msos/dynamic_linker/loaded_module.hpp"

#include "msos/usart_printer.hpp"

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
    writer << "Executing address: 0x" << hex << start_address_ << endl;
    return call_external(start_address_);
}

} // namespace dl
} // namespace msos

