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

#include "shell/command_parser.hpp"

MSTEST(CommandParserShould, ParseCommandAndArgs)
{
    char command[] = "ls -pa /rom";
    msos::shell::CommandParser sut(command, sizeof(command));

    mstest::expect_eq(sut.get_command(), "ls");
    mstest::expect_eq(sut.get_next_argument(), "-pa");
    mstest::expect_eq(sut.get_next_argument(), "/rom");
    mstest::expect_eq(sut.get_next_argument(), "");
    mstest::expect_eq(sut.get_command(), "ls");
}

MSTEST(CommandParserShould, ParseCommandAndArgsWithAdditionalSpaces)
{
    char command[] = "    ls    -pa     /rom   bom  ";
    msos::shell::CommandParser sut(command, sizeof(command));

    mstest::expect_eq(sut.get_command(), "ls");
    mstest::expect_eq(sut.get_next_argument(), "-pa");
    mstest::expect_eq(sut.get_next_argument(), "/rom");
    mstest::expect_false(sut.empty());
    mstest::expect_eq(sut.get_next_argument(), "bom");
    mstest::expect_true(sut.empty());
    mstest::expect_eq(sut.get_next_argument(), "");
    mstest::expect_eq(sut.get_command(), "ls");
}

MSTEST(CommandParserShould, ReturnEmptyCommandAndArgs)
{
    char command[] = "";
    msos::shell::CommandParser sut(command, sizeof(command));

    mstest::expect_eq(sut.get_command(), "");
    mstest::expect_eq(sut.get_next_argument(), "");
}
