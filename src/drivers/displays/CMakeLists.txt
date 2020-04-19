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


add_library(msos_drivers_displays STATIC)

set (library_includes ${PROJECT_SOURCE_DIR}/include/msos/drivers/displays)

target_sources(msos_drivers_displays
    PUBLIC
        ${library_includes}/ssd1306/ssd1306.hpp
        ${library_includes}/ssd1306/ssd1306_file.hpp

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ssd1306/ssd1306_file.cpp
)

target_include_directories(msos_drivers_displays
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(msos_drivers_displays
    PUBLIC
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a>
        $<$<CONFIG:DEBUG>:-Og -g>
        $<$<CONFIG:RELEASE>:-Os>
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-Werror -Wall -Wextra -Wpedantic -Wcast-align -Wunused -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces>
)

target_link_libraries(msos_drivers_displays
    PRIVATE
        eul
        msos_printer
        board
)
