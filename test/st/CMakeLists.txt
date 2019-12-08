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
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env/bin/pip install -r ${PROJECT_SOURCE_DIR}/test/st/requirements.txt --upgrade
    COMMAND cmake -E touch ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env.stamp
)

include(ModuleWrapperGenerator)
add_module_flags_target()

set(BINARIES_FOR_ST "" CACHE INTERNAL "")

add_custom_target(generate_config_file)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(dynamic_linker)

add_custom_command(
    TARGET generate_config_file 
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env.stamp
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env/bin/python3 ${CMAKE_CURRENT_SOURCE_DIR}/generate_config_file.py "${BINARIES_FOR_ST}"
    VERBATIM)

add_custom_target(run_st
    COMMAND 
    PYTHONPATH=${PYTHONPATH}:${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env/bin/py.test ${PROJECT_SOURCE_DIR}/test/st -s 
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/system_tests_env.stamp ${run_st_deps} generate_config_file
    VERBATIM
)

