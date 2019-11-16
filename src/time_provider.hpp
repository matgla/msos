#pragma once 

#include <eul/time/i_time_provider.hpp>

class TimeProvider : public eul::time::i_time_provider 
{
public: 
    std::chrono::milliseconds milliseconds() const override 
    {
        return std::chrono::milliseconds{0};
    }
};

