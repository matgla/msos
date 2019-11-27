add_library(example_module OBJECT)

target_sources(example_module
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set (module_linker_flags "${hal_linker_flags};-T${PROJECT_SOURCE_DIR}/linker_scripts/dynamic_module.ld;-nostartfiles;-nodefaultlibs;-nostdlib;-Wl,--unresolved-symbols=ignore-in-object-files;-Wl,--emit-relocs")
set (module_compiler_flags
    "${hal_cxx_compilation_flags};-msingle-pic-base;-fno-inline;-fPIE;-mno-pic-data-is-text-relative;-fomit-frame-pointer;-fno-inline;-mlong-calls")

target_compile_options(example_module PUBLIC ${module_compiler_flags})
target_link_options(example_module PUBLIC ${module_linker_flags})

include(ModuleWrapperGenerator)

find_program(VIRTUALENV virtualenv)

configure_virtual_env()

add_custom_target(
    example_module_wrapper_generator
    DEPENDS example_module venv.stamp
)

add_custom_command(
    TARGET example_module_wrapper_generator
    POST_BUILD
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/venv/bin/python3 ${PROJECT_SOURCE_DIR}/scripts/generate_wrappers.py
    generate_wrapper_code --output ${PROJECT_BINARY_DIR}/example_module_wrapper --input
    ${PROJECT_BINARY_DIR}/src/modules/example/CMakeFiles/example_module.dir --objcopy=${CMAKE_OBJCOPY}
    --module_name=example_module
    VERBATIM
)

add_library(example_module_wrapper OBJECT)

set_source_files_properties(${PROJECT_BINARY_DIR}/example_module_wrapper/wrapped_symbols.s PROPERTIES GENERATED 1)

target_sources(example_module_wrapper
    PRIVATE
    ${PROJECT_BINARY_DIR}/example_module_wrapper/wrapped_symbols.s
)
add_dependencies(example_module_wrapper example_module_wrapper_generator)

target_compile_options(example_module_wrapper PUBLIC ${hal_cxx_compilation_flags} -x assembler-with-cpp)
target_link_options(example_module_wrapper PUBLIC ${module_linker_flags})

add_executable(example_module_shared)

target_link_libraries(example_module_shared
    PUBLIC
        example_module_wrapper
        example_module
)

target_compile_options(example_module_shared PUBLIC ${module_compiler_flags})
target_link_options(example_module_shared PUBLIC ${module_linker_flags})

add_custom_command(
    TARGET example_module_shared
    POST_BUILD
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/venv/bin/python3 ${PROJECT_SOURCE_DIR}/scripts/generate_binary.py
    generate_wrapper_code --elf_filename=$<TARGET_FILE:example_module_shared> --module_name=example_module --objcopy=${CMAKE_OBJCOPY}
    VERBATIM
)
