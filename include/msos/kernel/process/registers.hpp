// This file is part of MSOS project. This is simple OS for embedded development devices.
// Copyright (C) 2019 Mateusz Stadnik
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

namespace msos
{
namespace kernel
{
namespace process
{

struct SoftwareStoredRegisters
{
    std::size_t r4;
    std::size_t r5;
    std::size_t r6;
    std::size_t r7;
    std::size_t r8;
    std::size_t r9;
    std::size_t r10;
    std::size_t r11;
    std::size_t lr;
};

struct HardwareStoredRegisters
{
    std::size_t r0;
    std::size_t r1;
    std::size_t r2;
    std::size_t r3;
    std::size_t r12;
    std::size_t lr;
    std::size_t pc;
    std::size_t psr;
};

struct RegistersDump
{
    // std::size_t r0;
    std::size_t r1;
    std::size_t r2;
    std::size_t r3;
    std::size_t r4;
    std::size_t r5;
    std::size_t r6;
    std::size_t r7;
    std::size_t r8;
    std::size_t r9;
    std::size_t r10;
    std::size_t r11;
    std::size_t r12;
    std::size_t lr;
};

} // namespace process
} // namespace kernel
} // namespace msos

