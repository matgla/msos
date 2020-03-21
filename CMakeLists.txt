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

cmake_minimum_required(VERSION 3.9)

project(msos NONE)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(${PROJECT_SOURCE_DIR}/cmake/GitModules.cmake)

set (user_boards_path ${PROJECT_SOURCE_DIR}/boards CACHE STRING "Path to boards provided from user" FORCE)
add_definitions("-DGSL_TERMINATE_ON_CONTRACT_VIOLATION=1 -DGSL_MSVC_USE_STL_NOEXCEPTION_WORKAROUND=1")

if (BOARD)
    fetch_module_via_branch(lib/hal ${PROJECT_SOURCE_DIR}/lib/hal ${CMAKE_CURRENT_SOURCE_DIR} "master")
    fetch_module_via_branch(lib/msgui ${PROJECT_SOURCE_DIR}/lib/msgui ${CMAKE_CURRENT_SOURCE_DIR} "master")
    set (DISABLE_ROMFS_TESTS "1")
    set (romfs_cxx_compilation_flags ${hal_cxx_compilation_flags})
    fetch_module_via_branch(lib/romfs ${PROJECT_SOURCE_DIR}/lib/romfs ${CMAKE_CURRENT_SOURCE_DIR} "master")
    fetch_module_via_branch(lib/gsl/gsl ${PROJECT_SOURCE_DIR}/lib/gsl/gsl ${CMAKE_CURRENT_SOURCE_DIR} "v2.0.0")
endif ()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
set(CMAKE_EXECUTABLE_SUFFIX "${CMAKE_EXECUTABLE_SUFFIX}")

enable_language(ASM C CXX)

if (BOARD)
    add_subdirectory(include/msos/drivers)
    add_subdirectory(src)
endif ()

if (NOT DISABLE_TESTS OR DISABLE_ST_TESTS)
    add_subdirectory(test)
endif ()
