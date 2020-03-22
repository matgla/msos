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

add_library(msos_kernel_process)

set (library_includes ${PROJECT_SOURCE_DIR}/include/msos/kernel/process)

target_sources(msos_kernel_process
    PUBLIC
        ${library_includes}/context_switch.hpp
        ${library_includes}/fwd.hpp
        ${library_includes}/process.hpp
        ${library_includes}/process_manager.hpp
        ${library_includes}/registers.hpp
        ${library_includes}/scheduler.hpp
        ${library_includes}/spawn.hpp

   PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/context_switch.s
        ${CMAKE_CURRENT_SOURCE_DIR}/process.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/process_manager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scheduler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/spawn.cpp
)

target_link_libraries(msos_kernel_process
    PUBLIC
        hal
)

target_include_directories(msos_kernel_process
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

