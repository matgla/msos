#include "msos/dynamic_linker/module_data.hpp"

namespace msos
{
namespace dl 
{

void ModuleData::allocate_text(const std::size_t size)
{
    text_.reserve(size);
}

void ModuleData::allocate_rodata(const std::size_t size)
{
    rodata_.reserve(size);
}

void ModuleData::allocate_data(const std::size_t size)
{
    data_.reserve(size);
}

ModuleData::DataSpan ModuleData::get_text()
{
    return gsl::make_span(text_);
}

ModuleData::DataSpan ModuleData::get_rodata()
{
    return gsl::make_span(rodata_);
}

ModuleData::DataSpan ModuleData::get_data()
{
    return gsl::make_span(data_);
}

} // namespace dl 
} // namespace msos 

