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

add_library(msos STATIC)

add_subdirectory(dynamic_linker)
add_subdirectory(kernel)
add_subdirectory(syscalls)
add_subdirectory(utils)
add_subdirectory(posix)
add_subdirectory(fs)
add_subdirectory(drivers)

set (library_includes ${PROJECT_SOURCE_DIR}/include/msos)

target_sources(msos
    PUBLIC
        ${library_includes}/usart_printer.hpp
)

target_link_libraries(msos
    PUBLIC
        msos_kernel_synchronization
        msos_syscalls
        msos_dynamic_linker
        msos_drivers_storage
        msos_fs
        hal
        msos_posix
)

target_include_directories(msos
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

add_executable(msos_test)

target_sources(msos_test
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(msos_test
    PUBLIC
        msos
)

set_target_properties(msos_test
    PROPERTIES
        SUFFIX ".elf"
)

include (GenerateBinary)
generate_binary(msos_test ${vendor})

