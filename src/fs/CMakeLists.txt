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

add_library(msos_fs)

set(library_includes ${PROJECT_SOURCE_DIR}/include/msos/fs)

target_sources(msos_fs
    PUBLIC
        ${library_includes}/i_file.hpp
        ${library_includes}/i_filesystem.hpp
        ${library_includes}/mount_point.hpp
        ${library_includes}/mount_points.hpp
        ${library_includes}/vfs.hpp
        ${library_includes}/ramfs.hpp
        ${library_includes}/romfs.hpp
        ${library_includes}/ramfs_file.hpp
        ${library_includes}/romfs_file.hpp
        ${library_includes}/usart_file.hpp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/mount_points.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ramfs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/romfs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ramfs_file.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/romfs_file.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/usart_file.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vfs.cpp
)

target_include_directories(msos_fs
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(msos_fs
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a -Wall -Wextra -Wpedantic -Wconversion -Wcast-align -Wunused -Wshadow -Wold-style-cast -Wpointer-arith -Wcast-qual -Wno-missing-braces>
        $<$<CONFIG:DEBUG>:-Og -g>
        $<$<CONFIG:RELEASE>:-Os>
)

target_link_libraries(msos_fs
    PUBLIC
        gsl
        eul
        msos_drivers_storage
        romfs
    PRIVATE
        hal
)


