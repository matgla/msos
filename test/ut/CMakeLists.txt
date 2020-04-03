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

cmake_minimum_required(VERSION 3.10)

include (${PROJECT_SOURCE_DIR}/cmake/GitModules.cmake)
if (NOT TARGET gtest)
    fetch_module_via_tag(test/ut/lib/googletest ${PROJECT_SOURCE_DIR}/test/ut/lib/googletest ${PROJECT_SOURCE_DIR} "release-1.10.0")
endif()

add_subdirectory(dynamic_linker)
add_subdirectory(posix)
add_subdirectory(fs)

