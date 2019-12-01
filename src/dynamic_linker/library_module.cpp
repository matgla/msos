#include "msos/dynamic_linker/library_module.hpp"

namespace msos
{
namespace dl
{

int LibraryModule::execute(int argc, char *argv[]) const 
{
    return -1;    
}

int LibraryModule::execute() const 
{
    return -1;
}

} // namespace dl 
} // namespace msos 

