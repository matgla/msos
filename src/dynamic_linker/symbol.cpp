#include "msos/dynamic_linker/symbol.hpp"

namespace msos
{
namespace dl
{

std::string_view to_string(const SymbolVisibility v)
{
    switch (v)
    {
         case SymbolVisibility::internal: return "internal";
         case SymbolVisibility::exported: return "exported";
         case SymbolVisibility::external: return "external";
    }
    return "Unknown";
}

std::string_view to_string(const Section s)
{
    switch (s)
    {
        case Section::code: return "code";
        case Section::rodata: return "rodata";
        case Section::data: return "data";
    }
    return "Unknown";
}

const uint32_t Symbol::size() const
{
    return size_;
}

const SymbolVisibility Symbol::visibility() const
{
    return visibility_;
}

const Section Symbol::section() const
{
    return section_;
}

const std::string_view Symbol::name() const
{
    return std::string_view(reinterpret_cast<const char*>(this) + sizeof(uint32_t) * 2 + sizeof(SymbolVisibility) + sizeof(Section));
}

const Symbol* Symbol::next() const
{
    return this + size_;
}

const uint32_t Symbol::offset() const 
{
    return offset_;
}


} // namespace dl
} // namespace msos

