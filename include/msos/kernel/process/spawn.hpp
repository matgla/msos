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

#include <sys/types.h>

#include <msos/dynamic_linker/environment.hpp>

extern "C"
{
    pid_t spawn_exec(const char* path, void *arg, const SymbolEntry* entries, int number_of_entries, std::size_t stack_size);
    pid_t spawn(void (*start_routine) (void *), void *arg);
    pid_t spawn_root_process(void (*start_routine) (void *), void *arg, std::size_t stack_size);
    int exec(const char* path, int argc, char* argv[], const SymbolEntry* entries, int number_of_entries);
}
