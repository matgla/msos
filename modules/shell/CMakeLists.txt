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

add_library(msos_shell_lib STATIC)

target_sources(msos_shell_lib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)


# TODO: smarter way to inherit flags
target_link_libraries(msos_shell_lib
    PRIVATE
        module_flags
        msos_interface
)

target_link_options(msos_shell_lib
    PUBLIC
        "-static-libgcc"
)

add_module(msos_shell msos_shell_lib)

if (${arch} STREQUAL "ARM")
    set (suffix ".bin")
    set (prefix "")
else ()
    set (suffix ".so")
    set (prefix "lib")
endif ()

add_custom_command(
    TARGET msos_shell
    POST_BUILD
    COMMAND mkdir -p ${PROJECT_BINARY_DIR}/rootfs/bin
    COMMAND rm -f ${PROJECT_BINARY_DIR}/rootfs.img
    COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/${prefix}msos_shell${suffix} ${PROJECT_BINARY_DIR}/rootfs/bin
    COMMAND genromfs -f ${PROJECT_BINARY_DIR}/rootfs.img -d ${PROJECT_BINARY_DIR}/rootfs -v -V msos_romfs
)
