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

add_library(msos_kernel_synchronization)

set(library_includes ${PROJECT_SOURCE_DIR}/include/msos/kernel/synchronization)

target_sources(msos_kernel_synchronization
    PUBLIC
        ${library_includes}/atomic.hpp
        ${library_includes}/semaphore.hpp
        ${library_includes}/mutex.hpp

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/semaphore.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/mutex.cpp
)

target_link_libraries(msos_kernel_synchronization
    PRIVATE
        hal
)

target_include_directories(msos_kernel_synchronization
   PUBLIC
       ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(msos_kernel_synchronization
    PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a -Wcast-align -Wpointer-arith -Wcast-qual -Wno-missing-braces>
        $<$<CONFIG:DEBUG>:-Og -g>
        $<$<CONFIG:RELEASE>:-Os>
    )

