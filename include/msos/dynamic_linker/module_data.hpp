#pragma once 

#include <cstdint>
#include <vector>

#include <gsl/span>

namespace msos
{
namespace dl 
{

class ModuleData 
{
public:
    using DataSpan = gsl::span<uint8_t>;

    void allocate_text(const std::size_t size);
    void allocate_rodata(const std::size_t size);
    void allocate_data(const std::size_t size);

    DataSpan get_text();
    DataSpan get_rodata();
    DataSpan get_data();

public: 
    std::vector<uint8_t> text_;
    std::vector<uint8_t> rodata_;
    std::vector<uint8_t> data_;
};

} // namespace dl
} // namespace msos 

