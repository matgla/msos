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

# Configure SOC

message("mcu: ${mcu}")

find_package (Python COMPONENTS Interpreter)

execute_process(
    COMMAND ${Python_EXECUTABLE} ${PROJECT_SOURCE_DIR}/scripts/generate_soc.py --soc=${mcu} --input=${CMAKE_CURRENT_SOURCE_DIR} --output=${CMAKE_CURRENT_BINARY_DIR}/generated_soc
    RESULT_VARIABLE rc
)

include (${CMAKE_CURRENT_BINARY_DIR}/generated_soc/soc_config.cmake)

if (rc)
    message (FATAL_ERROR "SOC generation failed")
endif ()

if (${arch} STREQUAL "ARM")
    add_subdirectory(arm)
endif ()
