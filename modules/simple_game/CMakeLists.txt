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

add_library(simple_game_lib STATIC)

target_sources(simple_game_lib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)


# TODO: smarter way to inherit flags
target_link_libraries(simple_game_lib
    PRIVATE
        module_flags
)

add_module(simple_game simple_game_lib)

if (${arch} STREQUAL "ARM")
    set (suffix ".bin")
    set (prefix "")
else ()
    set (suffix ".so")
    set (prefix "lib")
endif ()

add_custom_command(
    TARGET simple_game
    POST_BUILD
    COMMAND mkdir -p ${PROJECT_BINARY_DIR}/rootfs/bin/games
    COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/${prefix}simple_game${suffix} ${PROJECT_BINARY_DIR}/rootfs/bin/games/
)
