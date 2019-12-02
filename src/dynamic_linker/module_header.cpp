#include "msos/dynamic_linker/module.hpp"

#include "msos/usart_printer.hpp"

namespace msos
{
namespace dl
{

const std::string_view ModuleHeader::cookie() const
{
    return std::string_view(cookie_, 4);
}

const uint32_t ModuleHeader::code_size() const
{
    return code_size_;
}

const uint32_t ModuleHeader::rodata_size() const
{
    return rodata_size_;
}

const uint32_t ModuleHeader::data_size() const
{
    return data_size_;
}

const uint32_t ModuleHeader::bss_size() const
{
    return bss_size_;
}

const uint16_t ModuleHeader::number_of_relocations() const
{
    return number_of_relocations_;
}

const uint16_t ModuleHeader::total_relocations() const
{
    return total_relocations_;
}

const std::string_view ModuleHeader::name() const
{
    return std::string_view(reinterpret_cast<const char*>(this) + sizeof(ModuleHeader));
}

const uint32_t ModuleHeader::size() const
{
    return sizeof(ModuleHeader) + get_aligned_length(name());
}

} // namespace dl
} // namespace msos

