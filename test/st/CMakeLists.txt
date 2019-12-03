find_program (virtualenv_exec virtualenv)
if (NOT virtualenv_exec)
    message(FATAL_ERROR "Couldn't find virtualenv")
endif ()

set(virtualenv_exec ${virtualenv_exec} -p python3)

add_custom_command(
    OUTPUT virtualenv_command
    COMMAND ${virtualenv_exec} system_tests_env
)

add_custom_command(
    OUTPUT system_tests_env.stamp
    DEPENDS virtualenv_command ${PROJECT_SOURCE_DIR}/test/st/requirements.txt
    COMMAND ./system_tests_env/bin/pip install -r ${PROJECT_SOURCE_DIR}/test/st/requirements.txt --upgrade
)

add_custom_target(run_st
    COMMAND ./system_tests_env/bin/py.test ${PROJECT_SOURCE_DIR}/test/st
    DEPENDS system_tests_env.stamp
    VERBATIM
)

include(ModuleWrapperGenerator)
add_module_flags_target()

add_subdirectory(dynamic_linker)

