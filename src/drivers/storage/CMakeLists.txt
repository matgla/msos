# This file is part of MSOS project.
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

add_library(msos_drivers_storage)

set(library_includes ${PROJECT_SOURCE_DIR}/include/msos/drivers/storage)

target_sources(msos_drivers_storage
    PUBLIC
        ${library_includes}/block_device.hpp
        ${library_includes}/ram_block_device.hpp

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/block_device.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ram_block_device.cpp
)

target_include_directories(msos_drivers_storage
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(msos_drivers_storage
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a -Wall -Werror -Wextra -Wpedantic -Wconversion -Wcast-align -Wunused -Wshadow -Wold-style-cast -Wpointer-arith -Wcast-qual -Wno-missing-braces>
        $<$<CONFIG:DEBUG>:-Og -g>
        $<$<CONFIG:RELEASE>:-Os>
)

target_link_libraries(msos_drivers_storage
    PUBLIC
        gsl
    PRIVATE
        eul
        hal_flags
)


