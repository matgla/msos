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

namespace msos
{
namespace shell
{

class Splitter
{
public:
    virtual ~Splitter() = default;
    Splitter(char* input, std::size_t size, char delimiter, bool insert_null_character);

    bool empty();

    std::string_view get_next_part();
    void clear();

protected:
    char* input_ = nullptr;
    std::size_t size_ = 0;

    std::string_view tmp_ = "";
    char delimiter_;
    bool insert_null_character_;
};


} // namespace shell
} // namespace msos
