#pragma once

#include "msos/dynamic_linker/loaded_module.hpp"

namespace msos 
{
namespace dl 
{

class ExecutableModule : public LoadedModule
{
public:
    ExecutableModule(std::unique_ptr<Module>&& module, const std::size_t start_address);

    int execute(int argc, char *argv[]) const override;
    int execute() const override; 

private:
    const std::size_t start_address_;    
};

} // namespace dl 
} // namespace msos
 
