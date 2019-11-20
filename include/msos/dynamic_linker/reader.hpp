#pragma once 

namespace msos 
{
namespace dl
{

class Reader
{
public:
    Reader(const uint8_t* address) 
        : address_(address)
        , offset_(0)
    {
    }

    template <typename T>
    T read()
    {
        T* value = reinterpret_cast<T*>(address_ + offset_);
        offset_ += sizeof(T);
        return *value;
    }

    std::string_view read_string()
    {
        std::string_view str(reinterpret_cast<const char*>(address_ + offset_));
        offset_ += str.length();
        return str;
    }

    std::size_t get_number_of_readed_bytes() const 
    {
        return offset_;
    }

private:
    const uint8_t* address_;
    uint32_t offset_;
};

} // namespace dl
} // namespace msos 

