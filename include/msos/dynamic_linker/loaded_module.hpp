#pragma once

#include <memory>

#include "msos/dynamic_linker/module.hpp"
#include "msos/dynamic_linker/module_data.hpp"

namespace msos 
{
namespace dl 
{

class LoadedModule 
{
public:
    virtual ~LoadedModule() = default;
    LoadedModule(std::unique_ptr<Module>&& module);
    
    const Module& get_module() const;
    Module& get_module();

    virtual int execute(int argc, char *argv[]) const = 0;
    virtual int execute() const = 0;
protected:
    std::unique_ptr<Module> module_;
};

} // namespace dl 
} // namespace msos
 
