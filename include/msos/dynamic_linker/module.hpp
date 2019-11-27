#pragma once

#include <cstdint>
#include <string_view>

#include <gsl/span>

#include "msos/dynamic_linker/module_header.hpp"

namespace msos
{
namespace dl
{

class Module
{
public:
    using DataSpan = gsl::span<uint8_t>;
    Module(const ModuleHeader* module_header, const DataSpan& text,
        const DataSpan& rodata, const DataSpan& data);

    const DataSpan& text() const;
    const DataSpan& rodata() const;
    const DataSpan& data() const;

private:
    const ModuleHeader* module_header_;
    const DataSpan text_;
    const DataSpan rodata_;
    const DataSpan data_;
};

} // namespace dl
} // namespace msos

