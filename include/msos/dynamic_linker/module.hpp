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
    using DataSpan = gsl::span<uint8_t>;
    Module(const ModuleHeader& module_header);

    DataSpan get_text() const;
    DataSpan get_data() const;
    const ModuleHeader& get_header() const;
    const std::unique_ptr<uint32_t[]>& get_lot() const;
    std::unique_ptr<uint32_t[]>& get_lot();
    ModuleData& get_module_data();
    const ModuleData& get_module_data() const;

    void set_text(const DataSpan& text);
    void set_data(const DataSpan& data);

    void allocate_text();
    void allocate_data();

private:
    const ModuleHeader& module_header_;
    uint8_t* text_;
    uint8_t* data_;
    std::unique_ptr<uint32_t[]> lot_;
    ModuleData module_data_;
};

} // namespace dl
} // namespace msos

