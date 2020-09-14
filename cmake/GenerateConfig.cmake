function (generate_config)
    find_file (GENERATE_CONFIG generate_config.py ${PROJECT_SOURCE_DIR}/scripts)

    # set( ENV{srctree} ${PROJECT_SOURCE_DIR})
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E env
            srctree=${PROJECT_SOURCE_DIR}
            BOARD_DIR=${PROJECT_SOURCE_DIR}/boards/**
            ${GENERATE_CONFIG} --input ${PROJECT_BINARY_DIR}/.config --output ${PROJECT_BINARY_DIR} --kconfig ${PROJECT_SOURCE_DIR}/Kconfig
        RESULT_VARIABLE rc
    )

    if (${rc})
        message (FATAL_ERROR "Generation of config from KConfig failed")
    endif ()

    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${GENERATE_CONFIG})
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${PROJECT_BINARY_DIR}/.config)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${PROJECT_BINARY_DIR}/config.cmake)
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${PROJECT_BINARY_DIR}/config.json)
    include (${PROJECT_BINARY_DIR}/config.cmake)

    set_source_files_properties(${PROJECT_BINARY_DIR}/config.cmake PROPERTIES
        GENERATED TRUE
    )

    set (hal_user_config_file "${PROJECT_BINARY_DIR}/config.json" CACHE INTERNAL "")

endfunction()
