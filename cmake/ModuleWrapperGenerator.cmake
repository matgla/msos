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

endfunction()

function (add_module module_name module_library)
    if (${arch} STREQUAL "ARM")
        configure_virtual_env()
        find_file (VIRTUALENV_FILE venv.stamp ${PROJECT_BINARY_DIR}/)

        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/wrapped_symbols.s
            COMMAND ${PROJECT_BINARY_DIR}/module_generator_env/bin/pip install -r ${PROJECT_SOURCE_DIR}/scripts/requirements.txt --upgrade -q -q -q
            COMMAND ${PROJECT_BINARY_DIR}/module_generator_env/bin/python3 ${PROJECT_SOURCE_DIR}/scripts/generate_wrappers.py
            generate_wrapper_code --output ${CMAKE_CURRENT_BINARY_DIR} --input
            ${CMAKE_CURRENT_BINARY_DIR} --objcopy=${CMAKE_OBJCOPY}
            --module_name=${module_library} --disable_logs
            DEPENDS ${module_library} $<TARGET_OBJECTS:${module_library}> ${VIRTUALENV_FILE} ${PROJECT_SOURCE_DIR}/scripts/requirements.txt
        )

        add_library(${module_name}_wrapper OBJECT)
        # set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/wrapped_symbols.s PROPERTIES GENERATED 1)

        target_sources(${module_name}_wrapper
            PUBLIC
                ${CMAKE_CURRENT_BINARY_DIR}/wrapped_symbols.s
        )

    #target_compile_options(${module_name}_wrapper PUBLIC -x assembler-with-cpp)
        target_link_libraries(${module_name}_wrapper
            PUBLIC
                $<TARGET_OBJECTS:${module_library}>
                module_flags
        )

        file (TOUCH empty.cpp)
        add_executable(${module_name} empty.cpp)
        target_link_libraries(${module_name}
            PUBLIC
                $<TARGET_OBJECTS:${module_name}_wrapper>
                $<TARGET_OBJECTS:${module_library}>
                module_flags
        )
        add_dependencies(${module_name} ${module_name}_wrapper)
        add_dependencies(module_flags ${PROJECT_SOURCE_DIR}/scripts/generate_binary.py)
        add_custom_command(
            TARGET ${module_name}
            POST_BUILD
            COMMAND ${PROJECT_BINARY_DIR}/module_generator_env/bin/python3 ${PROJECT_SOURCE_DIR}/scripts/generate_binary.py
            generate_wrapper_code --disable_logs --elf_filename=$<TARGET_FILE:${module_name}> --module_name=${module_name}
            --objcopy=${CMAKE_OBJCOPY} --as_executable
            COMMAND cmake -E touch ${CMAKE_CURRENT_BINARY_DIR}/${module_name}_generate_bin.stamp
            DEPENDS ${PROJECT_SOURCE_DIR}/scripts/generate_binary.py
        )
    elseif (${arch} STREQUAL "x86")
        add_library(${module_name} SHARED empty.cpp)
        target_link_libraries(${module_name}
            PUBLIC
                $<TARGET_OBJECTS:${module_library}>
                module_flags
        )
    endif()
endfunction ()

function (add_module_flags_target)
    if (TARGET module_flags)
        return()
    endif ()

    add_library(module_flags INTERFACE)

    if (${arch} STREQUAL "ARM")
        target_link_options(module_flags INTERFACE
            "${hal_linker_flags};-T${PROJECT_SOURCE_DIR}/linker_scripts/dynamic_module.ld;-nostartfiles;-nodefaultlibs;-nostdlib;-Wl,--unresolved-symbols=ignore-in-object-files;-Wl,--emit-relocs;-fvisibility=hidden")

        target_compile_options(module_flags INTERFACE
            "${hal_cxx_compilation_flags};-fvisibility-inlines-hidden;-fno-function-sections;-fno-data-sections;-fno-section-anchors;-msingle-pic-base;-mno-pic-data-is-text-relative;-fPIE;-mlong-calls;-fvisibility=hidden;")
    else ()
    target_compile_options(module_flags
        INTERFACE
            $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a>
            $<$<CONFIG:DEBUG>:-Og -g>
            $<$<CONFIG:RELEASE>:-Os>
    )
    endif ()
endfunction()
