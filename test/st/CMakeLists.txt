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

find_program (virtualenv_exec virtualenv)

if (NOT virtualenv_exec)
    message(FATAL_ERROR "Couldn't find virtualenv")
endif ()

set(virtualenv_exec ${virtualenv_exec} -p python3)

find_file (ST_TESTS_VIRTUALENV_FILE test_st_venv.stamp ${CMAKE_CURRENT_BINARY_DIR}/)
if (${ST_TESTS_VIRTUALENV_FILE} STREQUAL ST_TESTS_VIRTUALENV_FILE-NOTFOUND)
    execute_process(
        COMMAND ${virtualenv_exec} ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env
    )
    file (TOUCH ${CMAKE_CURRENT_BINARY_DIR}/test_st_venv.stamp)
    find_file (ST_TESTS_VIRTUALENV_FILE test_st_venv.stamp ${CMAKE_CURRENT_BINARY_DIR}/)
endif ()

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env.stamp
    DEPENDS ${ST_TESTS_VIRTUALENV_FILE} ${PROJECT_SOURCE_DIR}/test/st/requirements.txt
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env/bin/pip install -r ${PROJECT_SOURCE_DIR}/test/st/requirements.txt --upgrade -q -q -q
    COMMAND cmake -E touch ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env.stamp
)

include(ModuleWrapperGenerator)
add_module_flags_target()

set(BINARIES_FOR_ST "" CACHE INTERNAL "")

add_custom_target(generate_config_file)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(drivers)
add_subdirectory(dynamic_linker)
add_subdirectory(kernel)
add_subdirectory(synchronization)

add_custom_command(
    TARGET generate_config_file
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env.stamp
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env/bin/python3 ${CMAKE_CURRENT_SOURCE_DIR}/generate_config_file.py "${BINARIES_FOR_ST}"
    VERBATIM)

add_custom_target(run_st
    COMMAND
    PYTHONPATH=${PYTHONPATH}:${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env/bin/py.test ${PROJECT_SOURCE_DIR}/test/st -s --disable-warnings
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env.stamp ${run_st_deps} generate_config_file
    VERBATIM
)

ADD_TEST(
        NAME ctest_build_test_code
        COMMAND cmake --build ${CMAKE_BINARY_DIR} --target run_st)
# ADD_TEST(ctest_run_test_code test_code)
# SET_TESTS_PROPERTIES(ctest_run_test_code
                    #  PROPERTIES DEPENDS ctest_build_test_code)

add_test(
    NAME run_system_tests_a
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env/bin/py.test
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set_tests_properties(run_system_tests_a PROPERTIES DEPENDS generate_config_file)

# add_dependencies(ctest_build_test_code ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env.stamp ${run_st_deps} generate_config_file)

# add_test(execute_st run_st)
