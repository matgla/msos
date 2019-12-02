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
    LoadedModule(const ModuleHeader& header);

    const Module& get_module() const;
    Module& get_module();

    void set_start_address(const std::size_t start_address);
    int execute(int argc, char *argv[]) const;
    int execute() const;
private:
    std::size_t start_address_;
    Module module_;
};

} // namespace dl
} // namespace msos

