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

#include <eul/error/error_code.hpp>

#include <msos/dynamic_linker/dynamic_linker.hpp>
#include <msos/dynamic_linker/dynamic_linker_errors.hpp>
#include <msos/dynamic_linker/environment.hpp>

#include "test/ut/dynamic_linker/utils.hpp"

namespace test
{
namespace ut
{
namespace dynamic_linker
{

TEST(DynamicLinkerShould, ProcessBinaryFileWithExecuteInPlace)
{
    auto data = load_test_binary("test_binary.bin");
    msos::dl::DynamicLinker sut;
    msos::dl::Environment<1> env{
        msos::dl::SymbolAddress{"printf", &printf}
    };
    eul::error::error_code ec;
    auto* module = sut.load_module(reinterpret_cast<const std::size_t*>(data.data()), msos::dl::LoadingModeCopyData, env, ec);
    EXPECT_NE(module, nullptr);
    EXPECT_FALSE(ec);
}

TEST(DynamicLinkerShould, FailWhenCookieMismatch)
{
    auto data = load_test_binary("test_binary.bin");
    msos::dl::DynamicLinker sut;
    msos::dl::Environment<1> env{
        msos::dl::SymbolAddress{"printf", &printf}
    };
    data[3] = 'a';
    eul::error::error_code ec;
    auto* module = sut.load_module(reinterpret_cast<const std::size_t*>(data.data()), msos::dl::LoadingModeCopyData, env, ec);
    EXPECT_EQ(module, nullptr);
    EXPECT_EQ(ec, msos::dl::DynamicLinkerErrors::CookieValidationFailure);
}

TEST(DynamicLinkerShould, FailWhenExternalSymbolNotResolved)
{
    auto data = load_test_binary("test_binary.bin");
    msos::dl::DynamicLinker sut;
    msos::dl::Environment<1> env{
        msos::dl::SymbolAddress{"scanf", &scanf}
    };
    eul::error::error_code ec;
    auto* module = sut.load_module(reinterpret_cast<const std::size_t*>(data.data()), msos::dl::LoadingModeCopyData, env, ec);
    EXPECT_EQ(module, nullptr);
    EXPECT_EQ(ec, msos::dl::DynamicLinkerErrors::ExternalRelocationFailure);
}

} // namespace dynamic_linker
} // namespace ut
} // namespace test


