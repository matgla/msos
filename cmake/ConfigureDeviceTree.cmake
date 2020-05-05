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


function(add_device_tree config_file)
    message(STATUS "Configuring device tree from: ${config_file}")

    execute_process(
        COMMAND
        python3 ${PROJECT_SOURCE_DIR}/scripts/generate_device_tree.py --input ${config_file} --output ${CMAKE_CURRENT_BINARY_DIR}/device_tree
        RESULT_VARIABLE result
    )

    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${config_file})

    if(NOT result EQUAL 0)
        message(FATAL_ERROR "Device tree generator failed")
    endif()

    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/device_tree ${CMAKE_CURRENT_BINARY_DIR}/device_tree_bin)

endfunction()
