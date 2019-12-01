#pragma once 

#include "msos/dynamic_linker/loaded_module.hpp"

namespace msos 
{
namespace dl 
{

class LibraryModule : public LoadedModule 
{
public:
    using LoadedModule::LoadedModule;
    int execute(int argc, char *argv[]) const override;
    int execute() const override; 
};

} // namespace dl
} // namespace msos 

