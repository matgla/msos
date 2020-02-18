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

add_executable(process_creation_deletion_test)

target_sources(process_creation_deletion_test
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(process_creation_deletion_test
    PUBLIC
        msos
)

include (GenerateBinary)
generate_binary(process_creation_deletion_test ${vendor})

target_include_directories(process_creation_deletion_test
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

set_target_properties(process_creation_deletion_test
    PROPERTIES
        SUFFIX ".elf"
)

target_compile_options(process_creation_deletion_test
    PUBLIC
    $<$<CONFIG:DEBUG>:-Og -g>
    $<$<CONFIG:RELEASE>:-Os>
)

include(TestUtils)
register_test_binary(process_creation_deletion_test)

