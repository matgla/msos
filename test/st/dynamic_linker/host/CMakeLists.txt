# This file is part of MSOS project. This is simple OS for embedded development devices.
# Copyright (C) 2020 Mateusz Stadnik
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

add_executable(host)

target_sources(host
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(host
    PUBLIC
        msos
        hal
)

include (GenerateBinary)
generate_binary(host ${vendor})

target_include_directories(host
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

set_target_properties(host
    PROPERTIES
        SUFFIX ".elf"
)

find_program(truncate_exec truncate)

add_custom_command(
    TARGET host
    POST_BUILD
    COMMAND ${truncate_exec} --size 32K ${CMAKE_CURRENT_BINARY_DIR}/host.bin
    VERBATIM
)

set (HOST_BINARY_FILE "${CMAKE_CURRENT_BINARY_DIR}/host.bin" CACHE INTERNAL "")

target_compile_options(host
    PUBLIC
    $<$<CONFIG:DEBUG>:-Og -g>
    $<$<CONFIG:RELEASE>:-Os>
)

