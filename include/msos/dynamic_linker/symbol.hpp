#pragma once

#include <cstdint>
#include <string_view>

namespace msos
{
namespace dl
{

enum class SymbolVisibility : uint16_t
{
    internal = 0,
    exported = 1,
    external = 2
};

enum class Section : uint16_t
{
    code = 0,
    data = 1
};

std::string_view to_string(const SymbolVisibility v);
std::string_view to_string(const Section s);

class Symbol
{
public:
    const uint32_t size() const;
    const SymbolVisibility visibility() const;
    const Section section() const;
    const std::string_view name() const;
    const Symbol& next() const;
    const uint32_t offset() const;
private:
    uint32_t size_;
    SymbolVisibility visibility_;
    Section section_;
    uint32_t offset_;
};

} // namespace dl
} // namespace msos

