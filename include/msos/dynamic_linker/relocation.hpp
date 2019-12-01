#pragma once 

#include <cstdint>
#include <string_view>

#include "msos/dynamic_linker/symbol.hpp"

namespace msos
{
namespace dl
{

class Relocation 
{
public:
    const uint32_t index() const;
    const Symbol& symbol() const;
    constexpr static std::size_t size() 
    {
        return sizeof(uint32_t) * 2;
    }
    const Relocation& next() const;
private:
    uint32_t index_;
    uint32_t symbol_offset_;
};

} // namespace dl 
} // namespace msos 

