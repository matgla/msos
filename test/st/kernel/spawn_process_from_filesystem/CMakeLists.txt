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

add_executable(spawn_process_from_filesystem)

target_sources(spawn_process_from_filesystem
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(spawn_process_from_filesystem
    PUBLIC
        hal
        msos
)

include (GenerateBinary)
generate_binary(spawn_process_from_filesystem ${vendor})

target_include_directories(spawn_process_from_filesystem
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

set_target_properties(spawn_process_from_filesystem
    PROPERTIES
        SUFFIX ".elf"
)

target_compile_options(spawn_process_from_filesystem
    PUBLIC
    $<$<CONFIG:DEBUG>:-Og -g>
    $<$<CONFIG:RELEASE>:-Os>
)

include(TestUtils)
register_test_binary(spawn_process_from_filesystem)

set (executable_file "${CMAKE_CURRENT_BINARY_DIR}/spawn_process_from_filesystem.bin")

find_program(truncate_exec truncate)


add_custom_command(
    TARGET spawn_process_from_filesystem
    POST_BUILD
    DEPENDS  ${CMAKE_CURRENT_BINARY_DIR}/spawn_process_from_filesystem_generate_bin.stamp
    COMMAND ${truncate_exec} --size 40K ${executable_file}
    COMMAND cp ${executable_file} ${executable_file}.tmp
    COMMAND cat ${executable_file}.tmp ${CMAKE_CURRENT_SOURCE_DIR}/test.img > ${executable_file}
    VERBATIM
)

