#include "msos/dynamic_linker/module.hpp"

#include "msos/usart_printer.hpp"

namespace msos
{
namespace dl
{

const std::string_view Module::cookie() const 
{
    return std::string_view(cookie_, 4);
}

const uint32_t Module::code_size() const 
{
    return code_size_;
}

const uint32_t Module::rodata_size() const 
{
    return rodata_size_;
}

const uint32_t Module::data_size() const 
{
    return data_size_;
}

const uint32_t Module::bss_size() const 
{
    return bss_size_;
}

const uint16_t Module::number_of_relocations() const 
{
    return number_of_relocations_; 
}

const uint16_t Module::total_relocations() const 
{
    return total_relocations_;
}

const std::string_view Module::name() const 
{
    return std::string_view(reinterpret_cast<const char*>(this) + sizeof(Module));
}

const uint32_t Module::size() const 
{
    return sizeof(Module) + get_aligned_length(name());
}

} // namespace dl
} // namespace msos 

