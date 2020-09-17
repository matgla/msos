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

#include "msos/drivers/character/usart/usart_driver.hpp"

#include <memory>

#include <eul/utils/unused.hpp>
#include <hal/core/criticalSection.hpp>

#include <board.hpp>

#include "msos/drivers/character/usart/usart_file.hpp"

#include "msos/usart_printer.hpp"

namespace msos
{
namespace drivers
{
namespace character
{

UsartWriter writer;

UsartDriver::UsartDriver(int usart_number)
    : usart_number_(usart_number)
    , readed_before_newline_(0)
    , echo_(true)
{
}

void UsartDriver::load()
{
    board::USART_1::init(115200);
    board::USART_1::on_data([this](const uint8_t byte) {
        char c = static_cast<char>(byte);
        if (c == 127)
        {
            if (readed_before_newline_ > 0)
            {
                if (echo_)
                {
                    board::USART_1::write("\b \b");
                }
                --readed_before_newline_;
            }
            buffer_.push('\b');

            return;
        }
        else if (c == '\n' || c == '\r')
        {
            c = '\n';
            readed_before_newline_ = -1;
        }
        char data[] = {c, '\0'};
        if (echo_)
        {
            board::USART_1::write(data);
        }
        buffer_.push(c);
        ++readed_before_newline_;
    });
}

void UsartDriver::unload()
{

}

UsartDriver::BufferType& UsartDriver::buffer()
{
    return buffer_;
}

std::unique_ptr<fs::IFile> UsartDriver::file(std::string_view path, int flags)
{
    UNUSED1(flags);
    return std::make_unique<fs::UsartFile>(*this, path);
}

void UsartDriver::lock()
{
    hal::core::startCriticalSection();
}

void UsartDriver::unlock()
{
    hal::core::stopCriticalSection();
}

void UsartDriver::echo(bool enable)
{
    echo_ = enable;
}

void UsartDriver::write(const gsl::span<const uint8_t>& data)
{
    board::USART_1::write(data);
}

} // namespace character
} // namespace fs
} // namespace msos
