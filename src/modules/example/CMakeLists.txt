add_library(example_module OBJECT)

target_sources(example_module 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp 
)

target_compile_options(example_module PUBLIC "${hal_compilation_flags};-msingle-pic-base;-fno-inline;-fPIE;-mno-pic-data-is-text-relative")
target_link_options(example_module PUBLIC -T${PROJECT_SOURCE_DIR}/linker_scripts/dynamic_module.ld -nostartfiles -nodefaultlibs -nostdlib -Wl,--unresolved-symbols=ignore-in-object-files -Wl,--emit-relocs)

add_custom_target(example_module_wrapper_generator)
message("${CMAKE_OBJCOPY}")
add_custom_command(
    TARGET example_module_wrapper_generator 
    COMMAND ${PROJECT_SOURCE_DIR}/scripts/generate_binary.py generate_wrapper_code --output ${PROJECT_BINARY_DIR}/example_module_wrapper --input
    ${PROJECT_BINARY_DIR}/src/modules/example/CMakeFiles/example_module.dir 
    --objcopy=${CMAKE_OBJCOPY}
    DEPENDS example_module
)

add_dependencies(example_module_wrapper_generator example_module)

file (GLOB_RECURSE example_module_wrapper_sources ${PROJECT_BINARY_DIR}/src/*.s)

add_library(example_module_wrapper STATIC)
target_sources(example_module_wrapper
    PUBLIC 
        ${example_module_wrapper_sources}
        ${CMAKE_CURRENT_SOURCE_DIR}/empty.c
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
