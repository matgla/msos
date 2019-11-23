#include "msos/dynamic_linker/relocation.hpp"

namespace msos 
{
namespace dl
{

const uint32_t Relocation::index() const 
{
    return index_;
}

const Symbol* Relocation::symbol() const 
{
    const uint8_t* ptr = reinterpret_cast<const uint8_t*>(this);
    ptr += symbol_offset_;
    return reinterpret_cast<const Symbol*>(ptr);
}

const Relocation* Relocation::next() const 
{
    return this + 1;
}

} // namespace dl 
} // namespace msos

