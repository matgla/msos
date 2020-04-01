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

add_executable(host_with_args)

target_sources(host_with_args
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(host_with_args
    PUBLIC
        msos
)

include (GenerateBinary)
generate_binary(host_with_args ${vendor})

target_include_directories(host_with_args
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

set_target_properties(host_with_args
    PROPERTIES
        SUFFIX ".elf"
)

find_program(qemu_img qemu-img)

if (NOT qemu_img)
    message(FATAL_ERROR "qemu-img executable not found!")
endif ()

add_custom_command(
    TARGET host_with_args
    COMMAND ${qemu_img} resize ${CMAKE_CURRENT_BINARY_DIR}/host_with_args.bin 32K
    POST_BUILD
    VERBATIM
)

set (HOST_WITH_ARGS_BINARY_FILE "${CMAKE_CURRENT_BINARY_DIR}/host_with_args.bin" CACHE INTERNAL "")

target_compile_options(host_with_args
    PUBLIC
    $<$<CONFIG:DEBUG>:-Og -g>
    $<$<CONFIG:RELEASE>:-Os>
)

