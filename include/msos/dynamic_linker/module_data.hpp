#pragma once

#include <cstdint>
#include <memory>

#include <gsl/span>

namespace msos
{
namespace dl
{

class ModuleData
{
public:
    using DataSpan = gsl::span<uint8_t>;

    uint8_t* allocate_text(const std::size_t size);
    uint8_t* allocate_data(const std::size_t size);

public:
    std::unique_ptr<uint8_t[]> text_;
    std::unique_ptr<uint8_t[]> data_;
};

} // namespace dl
} // namespace msos

