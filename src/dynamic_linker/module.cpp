#include "msos/dynamic_linker/module.hpp"

namespace msos
{
namespace dl
{

Module::Module(const ModuleHeader& header)
    : module_header_(header)
{
}

const Module::DataSpan& Module::get_text() const 
{
    return text_;
}

const Module::DataSpan& Module::get_rodata() const 
{
    return rodata_;
}

const Module::DataSpan& Module::get_data() const 
{
    return data_;
}

const ModuleHeader& Module::get_header() const 
{
    return module_header_;
}

const std::vector<uint32_t>& Module::get_lot() const 
{
    return lot_;
}

std::vector<uint32_t>& Module::get_lot()
{
    return lot_;
}

ModuleData& Module::get_module_data()
{
    return module_data_;
}

const ModuleData& Module::get_module_data() const 
{
    return module_data_;
}

void Module::set_text(const Module::DataSpan& text)
{
    text_ = text;
}

void Module::set_rodata(const Module::DataSpan& rodata)
{
    rodata_ = rodata;
}

void Module::set_data(const Module::DataSpan& data)
{
    data_ = data;
}

} // namespace dl
} // namespace msos 

