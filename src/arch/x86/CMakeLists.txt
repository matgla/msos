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

add_library(msos_arch_x86)

target_sources(msos_arch_x86
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_scheduler.hpp

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_process.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/call_external.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/context_switch.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_process.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/cpp_scheduler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/syscalls.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/spawn.cpp
)

target_include_directories(msos_arch_x86
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(msos_arch_x86
    PRIVATE
        $<$<CONFIG:DEBUG>:-Og -g>
        $<$<CONFIG:RELEASE>:-Os>
)

target_link_libraries(msos_arch_x86
    PRIVATE
        hal
        msos_kernel_process
)

