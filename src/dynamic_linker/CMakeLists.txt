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

add_library(msos_dynamic_linker STATIC)

set (library_includes ${PROJECT_SOURCE_DIR}/include/msos/dynamic_linker)

target_sources(msos_dynamic_linker
    PUBLIC
        ${library_includes}/dynamic_linker.hpp
        ${library_includes}/environment.hpp
        ${library_includes}/loaded_module.hpp
        ${library_includes}/module.hpp
        ${library_includes}/module_data.hpp
        ${library_includes}/module_header.hpp
        ${library_includes}/relocation.hpp
        ${library_includes}/symbol.hpp

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/call_external.s
        ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_linker.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/loaded_module.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/module.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/module_data.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/module_header.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/relocation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/symbol.cpp
)

target_include_directories(msos_dynamic_linker
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(msos_dynamic_linker
    PUBLIC
        hal
        gsl
        msos_arch
)

