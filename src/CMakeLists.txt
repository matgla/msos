# This file is part of MSOS project. This is simple OS for embedded development devices.
# Copyright (C) 2019 Mateusz Stadnik
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

set (target_name "msos")

add_executable(msos)

target_sources(msos
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_subdirectory(dynamic_linker)
add_subdirectory(utils)

target_link_libraries(msos
    PUBLIC
        # hal
        msos_dynamic_linker
)

include (GenerateBinary)
generate_binary(${target_name} ${vendor})

target_include_directories(msos
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_subdirectory(modules/example)

set_target_properties(
    msos
    PROPERTIES
        SUFFIX ".elf"
)

target_compile_options(msos PUBLIC "-g"  "-Og" "-ggdb3")
