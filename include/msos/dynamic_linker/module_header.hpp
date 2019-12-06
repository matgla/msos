#pragma once

#include <cstdint>
#include <string_view>

namespace msos
{
namespace dl
{

class ModuleHeader
{
public:
    const std::string_view cookie() const;
    const uint32_t code_size() const;
    const uint32_t data_size() const;
    const uint32_t bss_size() const;
    const uint16_t number_of_relocations() const;
    const uint16_t total_relocations() const;
    const std::string_view name() const;
    const uint32_t size() const;
private:
    char cookie_[4];
    uint32_t code_size_;
    uint32_t data_size_;
    uint32_t bss_size_;
    uint16_t number_of_relocations_;
    uint16_t total_relocations_;
};

} // namespace dl
} // namespace msos

