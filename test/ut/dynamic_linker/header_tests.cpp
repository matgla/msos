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

#include <cstdio>
#include <iostream>

#include <msos/dynamic_linker/module_header.hpp>

#include "test/ut/dynamic_linker/utils.hpp"

namespace test
{
namespace ut
{
namespace dynamic_linker
{

TEST(HeaderShould, ProcessModuleHeader)
{
    auto data = load_test_binary("test_binary.bin");

    const msos::dl::ModuleHeader& sut = *reinterpret_cast<const msos::dl::ModuleHeader*>(data.data());
    EXPECT_EQ(sut.cookie(), "MSDL");
    EXPECT_EQ(sut.code_size(), 64);
    EXPECT_EQ(sut.data_size(), 0);
    EXPECT_EQ(sut.bss_size(), 0);
    EXPECT_EQ(sut.number_of_exported_relocations(), 0);
    EXPECT_EQ(sut.number_of_local_relocations(), 1);
    EXPECT_EQ(sut.number_of_data_relocations(), 0);
    EXPECT_EQ(sut.number_of_exported_symbols(), 1);
    EXPECT_EQ(sut.size(), 24);
}

TEST(HeaderShould, ProcessModuleHeaderWithData)
{
    auto data = load_test_binary("test_module_with_data.bin");

    const msos::dl::ModuleHeader& sut = *reinterpret_cast<const msos::dl::ModuleHeader*>(data.data());
    EXPECT_EQ(sut.cookie(), "MSDL");
    EXPECT_EQ(sut.code_size(), 468);
    EXPECT_EQ(sut.data_size(), 24);
    EXPECT_EQ(sut.bss_size(), 0);
    EXPECT_EQ(sut.number_of_local_relocations(), 15);
    EXPECT_EQ(sut.number_of_data_relocations(), 0);
    EXPECT_EQ(sut.number_of_exported_symbols(), 1);
    EXPECT_EQ(sut.size(), 24);
}

TEST(HeaderShould, ProcessModuleHeaderWithBss)
{
    auto data = load_test_binary("test_bss_section.bin");

    const msos::dl::ModuleHeader& sut = *reinterpret_cast<const msos::dl::ModuleHeader*>(data.data());
    EXPECT_EQ(sut.cookie(), "MSDL");
    EXPECT_EQ(sut.code_size(), 1008);
    EXPECT_EQ(sut.data_size(), 52);
    EXPECT_EQ(sut.bss_size(), 8);
    EXPECT_EQ(sut.number_of_external_relocations(), 2);
    EXPECT_EQ(sut.number_of_local_relocations(), 34);
    EXPECT_EQ(sut.number_of_data_relocations(), 0);
    EXPECT_EQ(sut.number_of_exported_symbols(), 1);
    EXPECT_EQ(sut.size(), 24);
}

} // namespace dynamic_linker
} // namespace ut
} // namespace test


