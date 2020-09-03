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
    : input_(input)
    , size_(size)
{
    tmp_ = std::string_view(input);
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

bool CommandParser::empty()
{
    return tmp_ == "";
}

std::string_view CommandParser::get_next_part()
{
    if (tmp_.empty())
    {
        return "";
    }

    tmp_ = tmp_.substr(tmp_.find_first_not_of(' '));
    auto suffix = tmp_.find_last_not_of(' ');
    tmp_.remove_suffix(tmp_.length() - suffix - 1);
    std::size_t space_position = tmp_.find(' ');

    if (space_position == std::string_view::npos)
    {
        if (tmp_.size())
        {
            std::string_view part = tmp_;
            tmp_ = "";
            return part;
        }
        return "";
    }

    std::string_view part = tmp_.substr(0, space_position);
    tmp_.remove_prefix(space_position + 1);

    // always created from char* so in fact can be modified,
    // std::string_view must be reinitialized after referenced string change
    const_cast<char*>(part.data())[part.size()] = 0;

    return part;
}

} // namespace shell
} // namespace msos
