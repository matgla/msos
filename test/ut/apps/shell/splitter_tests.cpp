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

#include <mstest/mstest.hpp>

#include "shell/splitter.hpp"

MSTEST(SplitterShould, SplitByDelimiter)
{
    char test[] = "a;b;c";
    msos::shell::Splitter sut(test, sizeof(test), ';', false);

    mstest::expect_false(sut.empty());
    mstest::expect_eq(sut.get_next_part(), "a");
    mstest::expect_eq(sut.get_next_part(), "b");
    mstest::expect_eq(sut.get_next_part(), "c");
    mstest::expect_eq(test[1], ';');
    mstest::expect_true(sut.empty());
}

MSTEST(SplitterShould, SplitByDelimiterWithTerminate)
{
    char test[] = "a:b:c";
    msos::shell::Splitter sut(test, sizeof(test), ':', true);

    mstest::expect_false(sut.empty());
    mstest::expect_eq(sut.get_next_part(), "a");
    mstest::expect_eq(sut.get_next_part(), "b");
    mstest::expect_eq(sut.get_next_part(), "c");
    mstest::expect_eq(test[1], '\0');
    mstest::expect_true(sut.empty());
}
