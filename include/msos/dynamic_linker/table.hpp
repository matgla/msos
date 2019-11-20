#pragma once 

#include <optional>

namespace msos
{
namespace dl
{

template <typename T> 
class Table 
{
public:
    Table(uint32_t number_of_elements, const uint32_t* base_address)
    : number_of_elements_(number_of_elements)
    , base_address_(base_address)
    {
    }

    std::optional<T> at(std::size_t index)
    {
        if (index >= number_of_elements_)
        {
            return {};
        }

        std::size_t offset = 0;
        T element(base_address_);
        for (std::size_t i = 0; i < index; ++i)
        {
            T next(base_address_ + offset);
            offset_ += next.get_size();
        }

        return T(base_address_ + offset);
    }

    std::optional<T> get_next(const T& prev) const
    {
        if (prev.get_index() == number_of_elements_ - 1)
        {
            return {};
        }

        return T{base_address_ + prev.get_size()};
    }
private:
    const uint32_t number_of_elements_;
    const uint32_t* base_address_;
};

} // namespace dl 
} // namespace msos 

