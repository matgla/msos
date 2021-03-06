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

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

file (GLOB config ${PROJECT_BINARY_DIR}/.config)

find_package (Python3 COMPONENTS Interpreter)

set (ENV{srctree} ${PROJECT_SOURCE_DIR})
set (ENV{BOARD_DIR} boards/**)

if (NOT config)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E env srctree=${PROJECT_SOURCE_DIR} BOARD_DIR=boards/** ${Python_EXECUTABLE} ${PROJECT_SOURCE_DIR}/scripts/kconfig/guiconfig.py ${PROJECT_SOURCE_DIR}/Kconfig
    )
endif ()

add_custom_target(menuconfig
    COMMAND ${CMAKE_COMMAND} -E env srctree=${PROJECT_SOURCE_DIR} BOARD_DIR=boards/** ${Python_EXECUTABLE} ${PROJECT_SOURCE_DIR}/scripts/kconfig/menuconfig.py ${PROJECT_SOURCE_DIR}/Kconfig
    VERBATIM
)

include (GenerateConfig)
generate_config()

include(FetchContent)

FetchContent_Declare(
  mspkg
  GIT_REPOSITORY https://github.com/matgla/mspkg.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(mspkg)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${mspkg_SOURCE_DIR}/cmake)
include (mspkg)

setup_mspkg(${mspkg_SOURCE_DIR})


set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

set (user_boards_path ${PROJECT_SOURCE_DIR}/boards CACHE STRING "Path to boards provided from user")
add_definitions("-DGSL_TERMINATE_ON_CONTRACT_VIOLATION=1 -DGSL_MSVC_USE_STL_NOEXCEPTION_WORKAROUND")

message ("BOARD: ${BOARD}")

# if (NOT BOARD OR BOARD STREQUAL "Unknown" OR BOARD STREQUAL "unittest")
#     set(BOARD "unittest" CACHE STRING "Target board" FORCE)
# endif()

if (BUILD_MSOS_UT)
    set (ENABLE_SEMIHOSTING ON)
else ()
    set (ENABLE_SEMIHOSTING OFF)
endif ()

# this configures board, which is needed for HAL
add_subdirectory(socs)

find_package(hal REQUIRED)

enable_language(ASM C CXX)

if (ENABLE_SEMIHOSTING)
    add_definitions(-DENABLE_SEMIHOSTING)
endif ()

find_package(EmbeddedCMake REQUIRED)
find_package(msgui REQUIRED)
find_package(romfs REQUIRED)
find_package(GSL REQUIRED)

# set(CMAKE_EXECUTABLE_SUFFIX "${CMAKE_EXECUTABLE_SUFFIX}")

include_directories(BEFORE SYSTEM ${PROJECT_SOURCE_DIR}/include/msos/posix)
include_directories(BEFORE SYSTEM ${PROJECT_SOURCE_DIR}/include/msos/libc)
include_directories(BEFORE SYSTEM ${PROJECT_SOURCE_DIR}/include/msos/os)

set(msos_system_include_directories CACHE INTERNAL "")
set(msos_system_include_directories ${msos_system_include_directories} ${PROJECT_SOURCE_DIR}/include/msos/posix CACHE INTERNAL "")
set(msos_system_include_directories ${msos_system_include_directories} ${PROJECT_SOURCE_DIR}/include/msos/libc CACHE INTERNAL "")
set(msos_system_include_directories ${msos_system_include_directories} ${PROJECT_SOURCE_DIR}/include/msos/os CACHE INTERNAL "")


include(ConfigureDeviceTree)

add_device_tree(${board_configuration_path}/device_tree.json)

add_subdirectory(src)
add_subdirectory(modules)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/scripts/generate_device_tree.py)

if (BUILD_MSOS_UT OR MSOS_ENABLE_ST)
    include (CTest)
    enable_testing()
    add_subdirectory(test)
endif ()
