# This file is part of MSOS project.
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

add_library(msos_arch INTERFACE)

if (${arch} STREQUAL "ARM")
    add_subdirectory(armv7-m)
    add_subdirectory(newlib)
    target_link_libraries(msos_arch INTERFACE msos_arch_armv7m msos_arch_newlib)
endif()

if (${arch} STREQUAL "x86")
    add_subdirectory(x86)
    target_link_libraries(msos_arch INTERFACE msos_arch_x86)
endif ()


