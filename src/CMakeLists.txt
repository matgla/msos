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

add_library(msos INTERFACE)

add_subdirectory(apps)
add_subdirectory(arch)
add_subdirectory(dynamic_linker)
add_subdirectory(kernel)
add_subdirectory(syscalls)
add_subdirectory(posix)
add_subdirectory(fs)
add_subdirectory(drivers)
add_subdirectory(utils)
add_subdirectory(libc)

add_library(msos_printer)

set (library_includes ${PROJECT_SOURCE_DIR}/include/msos)

target_sources(msos_printer
    PUBLIC
        ${library_includes}/usart_printer.hpp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/usart_printer.cpp
)

target_include_directories(msos_printer
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(msos_printer
    PRIVATE
        hal
)

target_compile_options(msos_printer
    PUBLIC
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a>
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-Werror -Wall -Wextra -Wpedantic -Wcast-align -Wshadow -Wpointer-arith -Wcast-qual -Wno-missing-braces>
        $<$<CONFIG:DEBUG>:-Og -g>
        $<$<CONFIG:RELEASE>:-Os>
)

target_link_libraries(msos
    INTERFACE
        msos_kernel_synchronization
        msos_kernel_memory
        msos_syscalls
        msos_dynamic_linker
        msos_drivers
        msos_drivers_storage
        msos_fs
        msos_libc
        msos_posix
        msos_arch
        msos_apps
        msos_printer
        hal_flags
        msgui
)

target_include_directories(msos
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)

add_library(msos_interface INTERFACE)
target_include_directories(msos_interface
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include)
target_include_directories(msos_interface SYSTEM BEFORE
    INTERFACE ${PROJECT_SOURCE_DIR}/include/msos/posix
)

if (${vendor} STREQUAL "STM32")

add_executable(msos_test)

target_sources(msos_test
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(msos_test
    PUBLIC
        msos
        msos_shell
        hal

)

set_target_properties(msos_test
    PROPERTIES
        SUFFIX ".elf"
)

include (GenerateBinary)
generate_binary(msos_test ${vendor})

find_program(truncate_exec truncate)
set (executable_file "${CMAKE_CURRENT_BINARY_DIR}/msos_test.bin")

add_custom_target(generate_romfs
    ALL
    DEPENDS msos_test simple_game msos_ls

)

add_custom_command(
    TARGET generate_romfs
    POST_BUILD
    COMMAND rm -f ${PROJECT_BINARY_DIR}/rootfs.img
    COMMAND genromfs -f ${PROJECT_BINARY_DIR}/rootfs.img -d ${PROJECT_BINARY_DIR}/rootfs -v -V msos_romfs
    COMMAND ${truncate_exec} --size 40K ${executable_file}
    COMMAND cp ${executable_file} ${executable_file}.tmp
    COMMAND cat ${executable_file}.tmp ${PROJECT_BINARY_DIR}/rootfs.img > ${executable_file}
    VERBATIM
)

endif ()

