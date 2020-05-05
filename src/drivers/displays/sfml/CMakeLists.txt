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


add_library(msos_drivers_displays_sfml STATIC)

set (library_includes ${PROJECT_SOURCE_DIR}/include/msos/drivers/displays/sfml)

target_sources(msos_drivers_displays_sfml
    PUBLIC
        ${library_includes}/sfml.hpp
        ${library_includes}/sfml_file.hpp

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/sfml.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/sfml_file.cpp
)

target_include_directories(msos_drivers_displays_sfml
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(msos_drivers_displays_sfml
    PUBLIC
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a>
        $<$<CONFIG:DEBUG>:-Og -g>
        $<$<CONFIG:RELEASE>:-Os>
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-Werror -Wall -Wextra -Wpedantic -Wcast-align -Wunused -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces>
)


find_library(SFML REQUIRED)

message("!!!!!!!!!!!!!!!!!! ${SFML_LIBRARIES}")

target_link_libraries(msos_drivers_displays_sfml
    PRIVATE
        eul
        msos_printer

    PUBLIC
        ${SFML_LIBRARIES}
        ${SFML_DEPENDENCIES}
        board
)

target_include_directories(msos_drivers_displays_sfml
    PRIVATE
        ${SFML_INCLUDE_DIR}
)
