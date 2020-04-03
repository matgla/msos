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

#include <gtest/gtest.h>

#include "msos/posix/dirent_utils.hpp"

namespace msos
{
namespace posix
{
namespace dirent_utils
{

TEST(DirentUtilsShould, CorrectlyReturnNextPartForRootBasedPath)
{
    std::string_view path1("/test/directory/to/some/file");
    std::string_view part = get_next_part(path1);
    EXPECT_EQ(path1, "/directory/to/some/file");
    EXPECT_EQ(part, "test");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "/to/some/file");
    EXPECT_EQ(part, "directory");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "/some/file");
    EXPECT_EQ(part, "to");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "/file");
    EXPECT_EQ(part, "some");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "");
    EXPECT_EQ(part, "file");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "");
    EXPECT_EQ(part, "");
}

TEST(DirentUtilsShould, CorrectlyReturnNextPartForRootBasedPathWhenALotOfAdditionalSlashes)
{
    std::string_view path1("/////test///directory//to///some//file///");
    std::string_view part = get_next_part(path1);
    EXPECT_EQ(path1, "///directory//to///some//file///");
    EXPECT_EQ(part, "test");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "//to///some//file///");
    EXPECT_EQ(part, "directory");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "///some//file///");
    EXPECT_EQ(part, "to");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "//file///");
    EXPECT_EQ(part, "some");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "///");
    EXPECT_EQ(part, "file");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "");
    EXPECT_EQ(part, "");
}

TEST(DirentUtilsShould, CorrectlyReturnNextPartForRelativeBasedPath)
{
    std::string_view path1("./test/../directory/to/.");
    std::string_view part = get_next_part(path1);
    EXPECT_EQ(path1, "/test/../directory/to/.");
    EXPECT_EQ(part, ".");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "/../directory/to/.");
    EXPECT_EQ(part, "test");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "/directory/to/.");
    EXPECT_EQ(part, "..");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "/to/.");
    EXPECT_EQ(part, "directory");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "/.");
    EXPECT_EQ(part, "to");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "");
    EXPECT_EQ(part, ".");

    part = get_next_part(path1);
    EXPECT_EQ(path1, "");
    EXPECT_EQ(part, "");
}

TEST(DirentUtilsShould, CorrectlyReturnNextPartForEmpty)
{
    std::string_view path1("");
    std::string_view part = get_next_part(path1);
    EXPECT_EQ(path1, "");
    EXPECT_EQ(part, "");

    path1 = "/";
    part = get_next_part(path1);
    EXPECT_EQ(path1, "");
    EXPECT_EQ(part, "");
}

} // namespace dirent_utils
} // namespace posix
} // namespace msos


