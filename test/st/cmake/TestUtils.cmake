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

function (register_system_test_binary name)
    add_custom_target(
        ${name}_generate_test_image
        COMMAND cat ${HOST_BINARY_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${name}.bin >
        ${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin 
        DEPENDS host ${name} 
        VERBATIM 
    )

    set (run_st_deps "${run_st_deps}" ${name}_generate_test_image CACHE INTERNAL "")

    list(FIND BINARIES_FOR_ST "${name}:${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin" test_in_list)

    if (${test_in_list} EQUAL -1)
        message (STATUS "Adding binary for test environment: ${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin")
        set(BINARIES_FOR_ST "${BINARIES_FOR_ST}" "${name}:${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin" CACHE INTERNAL "")
    endif()


endfunction()
