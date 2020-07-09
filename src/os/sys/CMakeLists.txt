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

add_library(msos_os_sys)

set (library_includes ${PROJECT_SOURCE_DIR}/include/msos/os/sys)

target_sources(msos_os_sys
    PUBLIC
        ${library_includes}/ioctl.h
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ioctl.cpp
)

target_include_directories(msos_os_sys
    PUBLIC
        ${library_includes}
        ${PROJECT_SOURCE_DIR}/include
)


target_compile_options(msos_os_sys
    PUBLIC
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a>
        $<$<CONFIG:DEBUG>:-Og -g>
        $<$<CONFIG:RELEASE>:-Os>
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a -Werror -Wall -Wextra -Wpedantic -Wconversion -Wcast-align -Wunused -Wshadow -Wold-style-cast -Wpointer-arith -Wcast-qual -Wno-missing-braces>

)

target_link_libraries(msos_os_sys
    PRIVATE
        msos_fs
        hal_flags
        eul_filesystem
)
