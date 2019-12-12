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

add_library(interface_and_classes_lib OBJECT)

target_sources(interface_and_classes_lib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/child_implementation_a.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/i_some_interface.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/implementation_a.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/implementation_b.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/process_test.hpp

        ${CMAKE_CURRENT_SOURCE_DIR}/child_implementation_a.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/implementation_a.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/implementation_b.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/process_test.cpp
)

target_compile_options(interface_and_classes_lib
    PUBLIC
    $<$<CONFIG:DEBUG>:-Og -g>
    $<$<CONFIG:RELEASE>:-Os>
)

target_link_libraries(interface_and_classes_lib
    PUBLIC
        module_flags
        msos_utils
)

add_module(interface_and_classes interface_and_classes_lib)

include(TestUtils)
register_system_test_binary(interface_and_classes)
