#include "msos/dynamic_linker/loaded_module.hpp"

#include <utility>

namespace msos 
{
namespace dl 
{

LoadedModule::LoadedModule(std::unique_ptr<Module>&& module)
    : module_(std::move(module))
{
}

const Module& LoadedModule::get_module() const 
{
    return *module_;
}

Module& LoadedModule::get_module() 
{
    return *module_;
}

} // namespace dl 
} // namespace msos

