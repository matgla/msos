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
    set(${mcu} "STM32F103C8T6" PARENT_SCOPE)
    set(${mcu_family} "STM32F1xx" PARENT_SCOPE)
    set(${arch} "ARM" PARENT_SCOPE)
    set(${vendor} "STM32" PARENT_SCOPE)
endfunction()


function(get_linker_script linker_script linker_scripts_directory)
    set (${linker_script} ${user_boards_path}/Stm32_Black_Pill/linker_script.ld PARENT_SCOPE)
    set (${linker_scripts_directory} ${user_boards_path}/Stm32_Black_Pill PARENT_SCOPE)
endfunction()

function(add_device_hal_library hal_device_library)
    message(STATUS "Configuring STM32_Black_Pill")
    set(hal_device_library board PARENT_SCOPE)
    add_library(board STATIC)

    target_sources(board PUBLIC
        PUBLIC
            ${user_boards_path}/Stm32_Black_Pill/board.hpp
        PRIVATE
            ${user_boards_path}/Stm32_Black_Pill/board.cpp
    )

    target_include_directories(board PUBLIC
        ${user_boards_path}/Stm32_Black_Pill)


    target_link_libraries(board
        PUBLIC
            hal_interface
        PRIVATE
            hal_devices_arm_stm32f103c8t6
    )

    include(${PROJECT_SOURCE_DIR}/devices/arm/stm32/stm32f1/stm32f103c8t6/configure_stm32f103c8t6.cmake)
    configure_device()
endfunction()
