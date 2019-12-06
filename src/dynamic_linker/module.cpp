#include "msos/dynamic_linker/module.hpp"

#include <gsl/span>

namespace msos
{
namespace dl
{

Module::Module(const ModuleHeader& header)
    : module_header_(header)
{
}

Module::DataSpan Module::get_text() const
{
    return gsl::make_span(text_, module_header_.code_size());
}


Module::DataSpan Module::get_data() const
{
    return gsl::make_span(data_, module_header_.data_size());
}

const ModuleHeader& Module::get_header() const
{
    return module_header_;
}

const std::unique_ptr<uint32_t[]>& Module::get_lot() const
{
    return lot_;
}

std::unique_ptr<uint32_t[]>& Module::get_lot()
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
    text_ = text.data();
}

void Module::set_data(const Module::DataSpan& data)
{
    data_ = data.data();
}

void Module::allocate_text()
{
    text_ = module_data_.allocate_text(module_header_.code_size());
}

void Module::allocate_data()
{
    data_ = module_data_.allocate_data(module_header_.data_size());
}

} // namespace dl
} // namespace msos

