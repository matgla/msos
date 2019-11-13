add_library(example_module OBJECT)

target_sources(example_module
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_compile_options(example_module PUBLIC "${hal_compilation_flags};-msingle-pic-base;-fno-inline;-fPIE;-mno-pic-data-is-text-relative")
target_link_options(example_module PUBLIC -T${PROJECT_SOURCE_DIR}/linker_scripts/dynamic_module.ld -nostartfiles -nodefaultlibs -nostdlib -Wl,--unresolved-symbols=ignore-in-object-files -Wl,--emit-relocs)


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

add_custom_target(
    example_module_wrapper_generator
    DEPENDS example_module venv.stamp
)

add_custom_command(
    TARGET example_module_wrapper_generator
    POST_BUILD
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/venv/bin/python3 ${PROJECT_SOURCE_DIR}/scripts/generate_binary.py
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

target_compile_options(example_module_wrapper PUBLIC -msingle-pic-base -fno-inline -fPIE -mno-pic-data-is-text-relative
    -x assembler-with-cpp)
target_link_options(example_module_wrapper PUBLIC -T${PROJECT_SOURCE_DIR}/linker_scripts/dynamic_module.ld
    -nostartfiles -nodefaultlibs -nostdlib -Wl,--unresolved-symbols=ignore-in-object-files -Wl,--emit-relocs)

add_library(example_module_shared SHARED)

target_link_libraries(example_module_shared
    PUBLIC
        example_module_wrapper
        example_module
)


# get_target_property(example_module_location  example_module LINK_LIBRARIES)
# message("Location: ${example_module_location}")

target_compile_options(example_module_shared PUBLIC
    "${hal_compilation_flags};-msingle-pic-base;-fno-inline;-fPIE;-mno-pic-data-is-text-relative")
target_link_options(example_module_shared PUBLIC -T${PROJECT_SOURCE_DIR}/linker_scripts/dynamic_module.ld -nostartfiles -nodefaultlibs -nostdlib -Wl,--unresolved-symbols=ignore-in-object-files -Wl,--emit-relocs)
