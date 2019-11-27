function (configure_virtual_env)
    find_program(VIRTUALENV virtualenv)

    if(NOT VIRTUALENV)
        message(FATAL_ERROR "Could not find `virtualenv` in PATH")
    endif()

    set(VIRTUALENV ${VIRTUALENV} -p python3)

    add_custom_command(
        OUTPUT venv
        COMMAND ${VIRTUALENV} venv
    )

    add_custom_command(
        OUTPUT venv.stamp
        DEPENDS venv ${PROJECT_SOURCE_DIR}/scripts/requirements.txt
        COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/scripts/requirements.txt requirements.txt
        COMMAND ./venv/bin/pip install -r requirements.txt --upgrade
    )
endfunction()