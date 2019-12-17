// This file is part of MSOS project. This is simple OS for embedded development devices.
// Copyright (C) 2019 Mateusz Stadnik
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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

    template <typename T>
    SymbolAddress(const std::string_view& name, const T* data)
        : name_(name)
        , address_(reinterpret_cast<uint32_t>(data))
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
