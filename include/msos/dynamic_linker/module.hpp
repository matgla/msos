#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

#include <gsl/span>

#include "msos/dynamic_linker/module_header.hpp"
#include "msos/dynamic_linker/module_data.hpp"

namespace msos
{
namespace dl
{

class Module
{
public:
    using DataSpan = gsl::span<const uint8_t>;
    Module(const ModuleHeader& module_header);

    const DataSpan& get_text() const;
    const DataSpan& get_rodata() const;
    const DataSpan& get_data() const;
    const ModuleHeader& get_header() const;
    const std::vector<uint32_t>& get_lot() const;
    std::vector<uint32_t>& get_lot();
    ModuleData& get_module_data();
    const ModuleData& get_module_data() const;

    void set_text(const DataSpan& text);
    void set_rodata(const DataSpan& rodata);
    void set_data(const DataSpan& data);

private:
    const ModuleHeader& module_header_;
    DataSpan text_;
    DataSpan rodata_;
    DataSpan data_;
    std::vector<uint32_t> lot_;
    ModuleData module_data_;
};

} // namespace dl
} // namespace msos

