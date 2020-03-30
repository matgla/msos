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

add_executable (dynamic_linker_tests)

target_sources(dynamic_linker_tests
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/utils.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_linker_tests.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/header_tests.cpp
)

target_link_libraries(dynamic_linker_tests
    PRIVATE
        gtest
        gmock
        gtest_main
        msos_dynamic_linker
)

target_include_directories(dynamic_linker_tests
    PRIVATE
        ${PROJECT_SOURCE_DIR}
)

add_custom_command(
    TARGET dynamic_linker_tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:dynamic_linker_tests> ${PROJECT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test_binary.bin ${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test_binary.bin ${PROJECT_BINARY_DIR}/test_binary.bin
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test_module_with_data.bin ${PROJECT_BINARY_DIR}/test_module_with_data.bin
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test_bss_section.bin ${PROJECT_BINARY_DIR}/test_bss_section.bin
)


