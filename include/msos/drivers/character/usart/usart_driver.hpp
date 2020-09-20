// This file is part of MSOS project.
// Copyright (C) 2020 Mateusz Stadnik
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

#include <vector>
#include <string_view>

#include <board.hpp>

#include <eul/container/ring_buffer.hpp>

#include "msos/drivers/i_driver.hpp"

namespace msos
{
namespace drivers
{
namespace character
{

struct UsartDriver : public IDriver
{
public:
    using BufferType = eul::container::ring_buffer<uint8_t, 64>;
    UsartDriver(int usart_number);

    void load() override;
    void unload() override;

    std::unique_ptr<fs::IFile> file(std::string_view path, int flags) override;

    hal::interfaces::Usart& get();
    BufferType& buffer();

    void lock();
    void unlock();

    void echo(bool enable);

// private:
    int usart_number_;
    int readed_before_newline_;
    BufferType buffer_;
    bool echo_;
};

} // namespace character
} // namespace fs
} // namespace msos
