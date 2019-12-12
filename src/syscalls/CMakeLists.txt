# This file is part of MSOS project. This is simple OS for embedded development devices.
# Copyright (C) 2019 Mateusz Stadnik
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

add_library(msos_syscalls STATIC)

set (library_includes ${PROJECT_SOURCE_DIR}/include/msos/syscalls)

target_sources(msos_syscalls
    PUBLIC
        ${library_includes}/syscalls.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/syscalls.cpp
)

target_include_directories(msos_syscalls
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(msos_syscalls
    PUBLIC
        hal
)

