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
{
}

void UsartDriver::load()
{
    board::interfaces::usarts()[usart_number_]->init(115200);
    board::interfaces::usarts()[usart_number_]->on_data([this](const uint8_t byte) {
        char c = static_cast<char>(byte);
        if (c == 127)
        {
            if (readed_before_newline_ > 0)
            {
                board::interfaces::usarts()[usart_number_]->write("\b \b");
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
        board::interfaces::usarts()[usart_number_]->write(data);
        buffer_.push(c);
        ++readed_before_newline_;
    });
}

void UsartDriver::unload()
{

}

hal::interfaces::Usart& UsartDriver::get()
{
    return *board::interfaces::usarts()[usart_number_];
}

UsartDriver::BufferType& UsartDriver::buffer()
{
    return buffer_;
}

std::unique_ptr<fs::IFile> UsartDriver::file(std::string_view path)
{
    return std::make_unique<fs::UsartFile>(*this, path);
}

} // namespace character
} // namespace fs
} // namespace msos
