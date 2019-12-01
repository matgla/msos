#include "msos/dynamic_linker/executable_module.hpp"

extern "C"
{
int call_external(uint32_t address);
}

namespace msos 
{
namespace dl 
{

ExecutableModule::ExecutableModule(std::unique_ptr<Module>&& module, const std::size_t start_address)
    : LoadedModule(std::move(module))
    , start_address_(start_address)
{
}

int ExecutableModule::execute(int argc, char *argv[]) const
{
    return -1;
}

int ExecutableModule::execute() const 
{
    return call_external(start_address_);
}

} // namespace dl 
} // namespace msos

