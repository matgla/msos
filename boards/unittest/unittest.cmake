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

function(get_device_info mcu mcu_family arch vendor)
    message(STATUS "Configuration of board: x86_unittest")
    set(${mcu} "none" PARENT_SCOPE)
    set(${mcu_family} "none" PARENT_SCOPE)
    set(${arch} "x86" PARENT_SCOPE)
    set(${vendor} "none" PARENT_SCOPE)
endfunction()

function(add_device_hal_library hal_device_library)
    set(${hal_device_library} "x86_unittest")
    set(hal_device_library board PARENT_SCOPE)
    add_library(board STATIC)

    set(source_path "${PROJECT_SOURCE_DIR}/boards/x86/mock")
    target_sources(board
        PUBLIC
            ${user_boards_path}/unittest/board.hpp
        PRIVATE
            ${user_boards_path}/unittest/board.cpp
    )

    target_include_directories(board PUBLIC
        ${user_boards_path}/unittest)

    include(${PROJECT_SOURCE_DIR}/devices/x86/mock/configure_mock.cmake)
    configure_device()

    target_link_libraries(board
        PUBLIC
            hal_interface
            hal_x86_mock
    )

    target_compile_options(board PUBLIC
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a>
        $<$<CONFIG:DEBUG>:-Og -g>
        $<$<CONFIG:RELEASE>:-Os>
    )

    add_library(hal_flags INTERFACE)
    set(hal_cxx_compilation_flags "-std=c++2a;" CACHE INTERNAL "HAL CXX FLAGS")

endfunction()
