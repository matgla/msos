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

function (convert_to_bytes from to)
    message ("Conv: ${from}")
    string(TOUPPER ${from} from)
    message("Conv: ${from}")

    string(FIND ${from} "G" unit REVERSE)
    if (NOT ${unit} EQUAL -1)
        message ("GIGABYTES")
        string(SUBSTRING ${from} 0 ${unit} value)
        math(EXPR value "1024 * 1024 * 1024 * ${value}" OUTPUT_FORMAT DECIMAL)
        message ("value: ${value}")
        set (${to} ${value} PARENT_SCOPE)
        return ()
    endif ()

    string(FIND ${from} "M" unit REVERSE)
    if (NOT ${unit} EQUAL -1)
        message ("MEGABYTES")
        string(SUBSTRING ${from} 0 ${unit} value)
        math(EXPR value "1024 * 1024 * ${value}" OUTPUT_FORMAT DECIMAL)
        message ("value: ${value}")
        set (${to} ${value} PARENT_SCOPE)
        return ()
    endif ()

    string(FIND ${from} "K" unit REVERSE)
    if (NOT ${unit} EQUAL -1)
        message ("KILOBYTES")
        string(SUBSTRING ${from} 0 ${unit} value)
        math(EXPR value "1024 * ${value}" OUTPUT_FORMAT DECIMAL)
        message ("value: ${value}")
        set (${to} ${value} PARENT_SCOPE)
        return ()
    endif ()

    string(FIND ${from} "B" unit REVERSE)
    if (NOT ${unit} EQUAL -1)
        message ("BYTES")
        string(SUBSTRING ${from} 0 ${unit} value)
        message ("value: ${value}")
        set (${to} ${value} PARENT_SCOPE)
        return ()
    endif ()


endfunction()

message (STATUS "Configuring linker script")

message (STATUS "FS: ${FLASH_FILESYSTEM_SIZE}")

if (NOT flash_address)
    set (flash_address "0x00000000")
endif ()

if (NOT flash_size)
    set (flash_size "0")
endif ()


set (flash_fs_size ${FLASH_FILESYSTEM_SIZE})

convert_to_bytes(${flash_size} flash_size_in_bytes)
convert_to_bytes(${flash_fs_size} flash_fs_size_in_bytes)

message ("${flash_size_in_bytes} - ${flash_fs_size_in_bytes}" )
math(EXPR flash_fs_offset "${flash_size_in_bytes} - ${flash_fs_size_in_bytes}" OUTPUT_FORMAT HEXADECIMAL)

if (NOT flash_fs_address)
    math(EXPR flash_fs_address "${flash_address} + ${flash_fs_offset}" OUTPUT_FORMAT HEXADECIMAL)
endif ()


if (NOT flash_fs_size)
    set (flash_fs_size "0")
endif ()

if (NOT ram_start_address)
    set (ram_start_address "0x00000000")
endif ()

if (NOT ram_size)
    set (ram_size "0")
endif ()

if (NOT ccm_address)
    set (ccm_address "0x00000000")
endif ()

if (NOT ccm_size)
    set (ccm_size "0")
endif ()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/linker_script.ld.in ${CMAKE_CURRENT_BINARY_DIR}/linker_script.ld)

set (linker_script ${CMAKE_CURRENT_BINARY_DIR}/linker_script.ld CACHE INTERNAL "" FORCE)
set (linker_scripts_directory ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "" FORCE)
