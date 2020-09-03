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

add_executable(apps_shell_tests)

find_package(mstest REQUIRED)

target_sources(apps_shell_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/command_parser_tests.cpp
)

target_link_libraries(apps_shell_tests
    PUBLIC
        msos_shell
        hal
        mstest
)

include (GenerateBinary)

set_target_properties(apps_shell_tests
    PROPERTIES
        SUFFIX ".elf"
)

generate_binary(apps_shell_tests ${vendor})
