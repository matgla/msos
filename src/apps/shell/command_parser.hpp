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

#include <string_view>

#include "shell/splitter.hpp"

namespace msos
{
namespace shell
{

class CommandParser : public Splitter
{
public:
    CommandParser(char* input, std::size_t size);

    std::string_view get_command();
    std::string_view get_next_argument();

private:

    std::string_view command_ = "";
};


} // namespace shell
} // namespace msos
