find_program (virtualenv_exec virtualenv)

if (NOT virtualenv_exec)
    message(FATAL_ERROR "Couldn't find virtualenv")
endif ()

set(virtualenv_exec ${virtualenv_exec} -p python3)

find_file (ST_TESTS_VIRTUALENV_FILE test_st_venv.stamp ${PROJECT_BINARY_DIR}/)
if (${ST_TESTS_VIRTUALENV_FILE} STREQUAL ST_TESTS_VIRTUALENV_FILE-NOTFOUND)
    execute_process(
        COMMAND ${virtualenv_exec} system_tests_env 
    )
    file (TOUCH ${PROJECT_BINARY_DIR}/test_st_venv.stamp)
    find_file (ST_TESTS_VIRTUALENV_FILE test_st_venv.stamp ${PROJECT_BINARY_DIR}/)
endif ()

add_custom_command(
    OUTPUT system_tests_env.stamp
    DEPENDS ${ST_TESTS_VIRTUALENV_FILE} ${PROJECT_SOURCE_DIR}/test/st/requirements.txt
    COMMAND ${PROJECT_BINARY_DIR}/system_tests_env/bin/pip install -r ${PROJECT_SOURCE_DIR}/test/st/requirements.txt --upgrade
)

add_custom_target(run_st
    COMMAND ${PROJECT_BINARY_DIR}/system_tests_env/bin/py.test ${PROJECT_SOURCE_DIR}/test/st
    DEPENDS system_tests_env.stamp
    VERBATIM
)

include(ModuleWrapperGenerator)
add_module_flags_target()

add_subdirectory(dynamic_linker)

