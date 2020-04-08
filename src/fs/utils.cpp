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

#include "msos/fs/utils.hpp"

#include <algorithm>

namespace msos
{
namespace fs
{

void insert_with_order(std::vector<std::unique_ptr<IFile>>& vec, std::unique_ptr<IFile>&& file)
{
    if (vec.empty())
    {
        vec.push_back(std::move(file));
        return;
    }

    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        if (std::lexicographical_compare(file->name().begin(), file->name().end(), (*it)->name().begin(), (*it)->name().end()))
        {
            vec.insert(it, std::move(file));
            return;
        }
    }

    vec.push_back(std::move(file));
}

} // namespace fs
} // namespace msos
