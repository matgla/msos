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


function(get_device_info mcu mcu_family arch vendor)
    message(STATUS "Configuration of board: Stm32_F429_Discovery")
    set(${mcu} "STM32F429ZI" PARENT_SCOPE)
    set(${mcu_family} "STM32F4xx" PARENT_SCOPE)
    set(${arch} "ARM" PARENT_SCOPE)
    set(${vendor} "STM32" PARENT_SCOPE)
endfunction()

function(get_linker_script linker_script linker_scripts_directory)
    set (${linker_script} ${user_boards_path}/Stm32_F429_Discovery/linker_script.ld PARENT_SCOPE)
    set (${linker_scripts_directory} ${user_boards_path}/Stm32_F429_Discovery PARENT_SCOPE)
endfunction()

function(add_device_hal_library hal_device_library)
    message(STATUS "Configuring STM32_F429I_Discovery")
    set(board_path ${user_boards_path}/Stm32_F429_Discovery CACHE STRING "Path to board configuration files" FORCE)

    set(hal_device_library board PARENT_SCOPE)
    add_library(board STATIC)

    target_sources(board PUBLIC
        PUBLIC
            ${user_boards_path}/Stm32_F429_Discovery/board.hpp
        PRIVATE
            ${user_boards_path}/Stm32_F429_Discovery/board.cpp
    )

    target_include_directories(board PUBLIC
        ${user_boards_path}/Stm32_F429_Discovery)

    target_link_libraries(board
        PUBLIC
            hal_interface
            hal_devices_arm_stm32_f429
    )

    target_include_directories(board
        PUBLIC
            ${user_boards_path}/Stm32_F429_Discovery
    )

    include(${PROJECT_SOURCE_DIR}/devices/arm/stm32/stm32f4/stm32f429/configure_stm32f429.cmake)
    configure_device()

    add_library(sysconfig STATIC)
    target_sources(sysconfig PUBLIC
        PUBLIC
            ${user_boards_path}/Stm32_F429_Discovery/config.hpp
        PRIVATE
            ${user_boards_path}/Stm32_F429_Discovery/config.cpp
    )
    target_include_directories(sysconfig PUBLIC
        ${user_boards_path}/Stm32_F429_Discovery
        ${PROJECT_SOURCE_DIR}/src)

    target_link_libraries(sysconfig
        PRIVATE
            msos_kernel_process
            msos_arch
            hal_interface
    )
endfunction()
