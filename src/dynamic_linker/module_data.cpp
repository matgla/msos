#include "msos/dynamic_linker/module_data.hpp"

namespace msos
{
namespace dl
{

uint8_t* ModuleData::allocate_text(const std::size_t size)
{
    text_.reset(new uint8_t[size]);
    return text_.get();
}

uint8_t* ModuleData::allocate_rodata(const std::size_t size)
{
    rodata_.reset(new uint8_t[size]);
    return rodata_.get();
}

uint8_t* ModuleData::allocate_data(const std::size_t size)
{
    data_.reset(new uint8_t[size]);
    return data_.get();
}

} // namespace dl
} // namespace msos

