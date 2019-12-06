function (configure_virtual_env)
    find_program (virtualenv_exec virtualenv)

    if(NOT virtualenv_exec)
        message(FATAL_ERROR "Couldn't find virtualenv")
    endif()

    set(virtualenv_exec ${virtualenv_exec} -p python3)
    find_file (VIRTUALENV_FILE venv.stamp ${PROJECT_BINARY_DIR}/)
    if (${VIRTUALENV_FILE} STREQUAL VIRTUALENV_FILE-NOTFOUND)
        execute_process(
            COMMAND ${virtualenv_exec} module_generator_env
        )
        file (TOUCH ${PROJECT_BINARY_DIR}/venv.stamp)
        find_file (VIRTUALENV_FILE venv.stamp ${PROJECT_BINARY_DIR}/)
    endif ()

    add_custom_command(
        OUTPUT module_generator_env.stamp
        DEPENDS ${VIRTUALENV_FILE} ${PROJECT_SOURCE_DIR}/scripts/requirements.txt
        COMMAND ${PROJECT_BINARY_DIR}/module_generator_env/bin/pip install -r ${PROJECT_SOURCE_DIR}/scripts/requirements.txt --upgrade
    )
endfunction()

function (add_wrapper module_name)
    configure_virtual_env()
    add_custom_target(
        ${module_name}_wrapper_generator
        DEPENDS ${module_name} module_generator_env.stamp
    )

    add_custom_command(
        TARGET ${module_name}_wrapper_generator
        POST_BUILD
        COMMAND ${PROJECT_BINARY_DIR}/module_generator_env/bin/python3 ${PROJECT_SOURCE_DIR}/scripts/generate_wrappers.py
        generate_wrapper_code --output ${PROJECT_BINARY_DIR}/${module_name}_wrapper --input
        ${CMAKE_CURRENT_BINARY_DIR} --objcopy=${CMAKE_OBJCOPY}
        --module_name=${module_name}
        VERBATIM
    )

    add_library(${module_name}_wrapper OBJECT)

    set_source_files_properties(${PROJECT_BINARY_DIR}/${module_name}_wrapper/wrapped_symbols.s PROPERTIES GENERATED 1)

    target_sources(${module_name}_wrapper
        PRIVATE
        ${PROJECT_BINARY_DIR}/${module_name}_wrapper/wrapped_symbols.s
    )
    add_dependencies(${module_name}_wrapper ${module_name}_wrapper_generator)

    target_compile_options(${module_name}_wrapper PUBLIC -x assembler-with-cpp)
    target_link_libraries(${module_name}_wrapper PUBLIC module_flags)

    add_executable(${module_name}_shared)

    target_link_libraries(${module_name}_shared
        PUBLIC
            ${module_name}_wrapper
            ${module_name}
            module_flags
    )


    add_custom_command(
        TARGET ${module_name}_shared
        POST_BUILD
        COMMAND ${PROJECT_BINARY_DIR}/module_generator_env/bin/python3 ${PROJECT_SOURCE_DIR}/scripts/generate_binary.py
        generate_wrapper_code --elf_filename=$<TARGET_FILE:${module_name}_shared> --module_name=${module_name} --objcopy=${CMAKE_OBJCOPY}
        VERBATIM
    )

endfunction ()

function (add_module_flags_target)
    if (TARGET module_flags)
        return()
    endif ()
    add_library(module_flags INTERFACE)

    target_link_options(module_flags INTERFACE "${hal_linker_flags};-T${PROJECT_SOURCE_DIR}/linker_scripts/dynamic_module.ld;-nostartfiles;-nodefaultlibs;-nostdlib;-Wl,--unresolved-symbols=ignore-in-object-files;-Wl,--emit-relocs")

    target_compile_options(module_flags INTERFACE "${hal_cxx_compilation_flags};-msingle-pic-base;-fno-inline;-fPIE;-mno-pic-data-is-text-relative;-fomit-frame-pointer;-fno-inline;-mlong-calls")



endfunction()
