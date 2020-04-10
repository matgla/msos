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

        # file (TOUCH ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp)
        # add_executable(${module_name} ${CMAKE_CURRENT_BINARY_DIR}/empty.cpp)
        # get_target_property(library_libs ${module_library} LINK_LIBRARIES)
        # message("Linking to library libs ${library_libs}")
        # target_link_libraries(${module_name}
        #     PUBLIC
        #         # ${module_name}_wrapper
        #         module_flags
        #         ${library_libs}
        # )

        add_custom_command(
            TARGET ${module_name}
            POST_BUILD
            COMMAND ${PROJECT_BINARY_DIR}/module_generator_env/bin/pip install -r ${PROJECT_SOURCE_DIR}/scripts/requirements.txt --upgrade -q -q -q
            COMMAND ${PROJECT_BINARY_DIR}/module_generator_env/bin/python3 ${PROJECT_SOURCE_DIR}/scripts/generate_binary.py
            generate_wrapper_code --disable_logs --elf_filename=$<TARGET_FILE:${module_name}> --module_name=${module_name}
            --objcopy=${CMAKE_OBJCOPY} --as_executable --api=${PROJECT_SOURCE_DIR}/api/symbol_codes.json
            DEPENDS ${PROJECT_SOURCE_DIR}/scripts/generate_binary.py
        )

        set_target_properties(${module_name} PROPERTIES OBJECT_DEPENDS ${PROJECT_SOURCE_DIR}/scripts/generate_wrappers.py)

    elseif (${arch} STREQUAL "x86")
    endif()
endfunction ()

function (add_module_flags_target)
    if (TARGET module_flags)
        return()
    endif ()

    add_library(module_flags INTERFACE)

    if (${arch} STREQUAL "ARM")
        target_link_options(module_flags INTERFACE
            "${hal_linker_flags};-T${PROJECT_SOURCE_DIR}/linker_scripts/dynamic_module.ld;-nostartfiles;-nodefaultlibs;-nostdlib;-Wl,--unresolved-symbols=ignore-in-object-files;-Wl,--emit-relocs;-fvisibility=hidden;")

        set(module_common_flags "-fvisibility-inlines-hidden;-fno-function-sections;-fno-data-sections;-fno-section-anchors;-msingle-pic-base;-mno-pic-data-is-text-relative;-fPIE;-mlong-calls;-fvisibility=hidden;")
        target_compile_options(module_flags INTERFACE
            $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a;${hal_cxx_compilation_flags};${module_common_flags}>
            $<$<COMPILE_LANGUAGE:C>:${hal_c_compilation_flags};${module_common_flags}>

            $<$<CONFIG:DEBUG>:-Og -g>
            $<$<CONFIG:RELEASE>:-Os>)
            else ()
    target_compile_options(module_flags
        INTERFACE
            $<$<COMPILE_LANGUAGE:CXX>:-std=c++2a;-fPIC>
            $<$<CONFIG:DEBUG>:-Og -g>
            $<$<CONFIG:RELEASE>:-Os>
    )
    endif ()
endfunction()
