#pragma once 

#include <cstdint>
#include <array>
#include <string_view>

namespace msos
{
namespace dl 
{

class SymbolAddress
{
public:
    template <typename R, typename... Args>
    SymbolAddress(const std::string_view& name, R(*function)(Args...))
        : name_(name)
        , address_(reinterpret_cast<uint32_t>(function))
    {

    }

    const std::string_view name() const 
    {
        return name_;
    }

    const uint32_t address() const 
    {
        return address_;
    }

private:
    const std::string_view name_;
    const uint32_t address_;
};
 
template <std::size_t N>
class Environment 
{
public:
    template <typename... Args>
    Environment(Args&&... args) : data_({std::forward<Args>(args)...})
    {
    }

    const SymbolAddress* find_symbol(const std::string_view& name) const 
    {
        for (const auto& symbol : data_)
        {
            if (symbol.name() == name)
            {
                return &symbol;
            }
        }
        return nullptr;
    }
private:
    std::array<SymbolAddress, N> data_;
};

} // namespace dl
} // namespace msos 

