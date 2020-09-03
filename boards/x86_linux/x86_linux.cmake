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

function(get_device_info mcu mcu_family arch vendor)
    message(STATUS "Configuration of board: KeychainGamer")
    set(${mcu} "X86" PARENT_SCOPE)
    set(${mcu_family} "X86" PARENT_SCOPE)
    set(${arch} "X86" PARENT_SCOPE)
    set(${vendor} "X86" PARENT_SCOPE)
endfunction()


function(get_linker_script linker_script linker_scripts_directory)
    set (${linker_scripts_directory} ${user_boards_path}/x86_linux PARENT_SCOPE)
endfunction()

function(add_device_hal_library hal_device_library)
    message(STATUS "Configuring x86_linux")
    set(hal_device_library board PARENT_SCOPE)
    add_library(board STATIC)

    target_sources(board PUBLIC
        PUBLIC
            ${user_boards_path}/x86_linux/board.hpp
        PRIVATE
            ${user_boards_path}/x86_linux/board.cpp
    )

    target_include_directories(board PUBLIC
        ${user_boards_path}/x86_linux
        ${PROJECT_SOURCE_DIR}/src)

    target_link_libraries(board
        PUBLIC
            hal_interface
            hal_x86_linux
    )


    include(${PROJECT_SOURCE_DIR}/devices/x86/linux/configure_linux.cmake)
    configure_device()
    set(board_path ${user_boards_path}/x86_linux CACHE STRING "Path to board configuration files" FORCE)

    add_library(hal_flags INTERFACE)
    set(hal_cxx_compilation_flags "-std=c++2a;" CACHE INTERNAL "HAL CXX FLAGS")

    target_compile_options(hal_flags INTERFACE
        $<$<COMPILE_LANGUAGE:CXX>:${hal_cxx_compilation_flags}>
        $<$<CONFIG:DEBUG>:-Og -g>
        $<$<CONFIG:RELEASE>:-Os>
    )

    add_library(sysconfig STATIC)
    target_sources(sysconfig PUBLIC
        PUBLIC
            ${user_boards_path}/x86_linux/config.hpp
        PRIVATE
            ${user_boards_path}/x86_linux/config.cpp
    )

    target_link_options(sysconfig
        PUBLIC
            -Wl,--wrap=write;
            -Wl,--wrap=open;
            -Wl,--wrap=read;
            -Wl,--wrap=kill;
            -Wl,--wrap=getpid;
            -Wl,--wrap=close;
            -Wl,--wrap=ioctl;
            -Wl,--wrap=fsync;
    )


    target_include_directories(sysconfig PUBLIC
        ${user_boards_path}/x86_linux
        ${PROJECT_SOURCE_DIR}/src)

    target_link_libraries(sysconfig
        PRIVATE
            msos_kernel_process
            msos_syscalls
            msos_arch
            msos_os_sys
            msos_posix
            board
    )

endfunction()
