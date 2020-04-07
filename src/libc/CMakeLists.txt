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

add_library(msos_libc)

set (library_includes ${PROJECT_SOURCE_DIR}/include/msos/kernel/memory)

target_sources(msos_libc
   PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/printf.cpp
)

target_include_directories(msos_libc
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(msos_libc
    PRIVATE
        msos_printer
        hal_flags
)
