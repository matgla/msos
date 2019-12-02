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

function (add_wrapper module_name)
    configure_virtual_env()
    add_custom_target(
        ${module_name}_wrapper_generator
        DEPENDS ${module_name} venv.stamp
    )

    add_custom_command(
        TARGET ${module_name}_wrapper_generator
        POST_BUILD
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/venv/bin/python3 ${PROJECT_SOURCE_DIR}/scripts/generate_wrappers.py
        generate_wrapper_code --output ${PROJECT_BINARY_DIR}/${module_name}_wrapper --input
        ${PROJECT_BINARY_DIR}/src/modules/example/CMakeFiles/${module_name}.dir --objcopy=${CMAKE_OBJCOPY}
        --module_name=${module_name}
        VERBATIM
    )

    add_library(${module_name}_wrapper OBJECT)

    set_source_files_properties(${PROJECT_BINARY_DIR}/example_module_wrapper/wrapped_symbols.s PROPERTIES GENERATED 1)

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
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/venv/bin/python3 ${PROJECT_SOURCE_DIR}/scripts/generate_binary.py
        generate_wrapper_code --elf_filename=$<TARGET_FILE:${module_name}_shared> --module_name=${module_name} --objcopy=${CMAKE_OBJCOPY}
        VERBATIM
    )

endfunction ()

function (add_module_flags_target)
    add_library(module_flags INTERFACE)
    
    target_link_options(module_flags INTERFACE "${hal_linker_flags};-T${PROJECT_SOURCE_DIR}/linker_scripts/dynamic_module.ld;-nostartfiles;-nodefaultlibs;-nostdlib;-Wl,--unresolved-symbols=ignore-in-object-files;-Wl,--emit-relocs")

    target_compile_options(module_flags INTERFACE "${hal_cxx_compilation_flags};-msingle-pic-base;-fno-inline;-fPIE;-mno-pic-data-is-text-relative;-fomit-frame-pointer;-fno-inline;-mlong-calls")



endfunction()
