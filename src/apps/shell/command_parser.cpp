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

#include "shell/command_parser.hpp"

#include <cstdio>

namespace msos
{
namespace shell
{

CommandParser::CommandParser(char* input, std::size_t size)
    : Splitter(input, size, ' ', true)
{
    command_ = get_next_part();
}

std::string_view CommandParser::get_command()
{
    return command_;
}

std::string_view CommandParser::get_next_argument()
{
    return get_next_part();
}

} // namespace shell
} // namespace msos
