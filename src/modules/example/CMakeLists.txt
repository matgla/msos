add_library(example_module STATIC)

target_sources(example_module 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp 
)

target_compile_options(example_module PUBLIC -msingle-pic-base -fno-inline -fPIE -mno-pic-data-is-text-relative)
target_link_options(example_module PUBLIC -T${PROJECT_SOURCE_DIR}/linker_scripts/dynamic_module.ld -nostartfiles -nodefaultlibs -nostdlib -Wl,--unresolved-symbols=ignore-in-object-files -Wl,--emit-relocs)

target_link_libraries(example_module PUBLIC stm32)

add_custom_target(example_module_wrapper_generator)

add_custom_command(
    TARGET example_module_wrapper_generator 
    COMMAND ${PROJECT_SOURCE_DIR}/scripts/generate_binary.py generate_wrapper_code --output ${PROJECT_BINARY_DIR}/example_module_wrapper --input $<TARGET_FILE:example_module>
    DEPENDS $<TARGET_FILE:example_module>
)

file (GLOB_RECURSE example_module_wrapper_sources *.s)

#add_library(example_module_wrapper STATIC)
# target_sources(example_module_wrapper
#     PUBLIC 
#         ${example_module_wrapper_sources}
#         ${CMAKE_CURRENT_SOURCE_DIR}/empty.c
# )
# 
# add_dependencies(example_module_wrapper example_module_wrapper_generator)
# 
# target_compile_options(example_module_wrapper PUBLIC -msingle-pic-base -fno-inline -fPIE -mno-pic-data-is-text-relative
#     -x assembler-with-cpp)
# target_link_options(example_module_wrapper PUBLIC -T${PROJECT_SOURCE_DIR}/linker_scripts/dynamic_module.ld
#     -nostartfiles -nodefaultlibs -nostdlib -Wl,--unresolved-symbols=ignore-in-object-files -Wl,--emit-relocs)   

add_library(example_module_shared SHARED)

target_link_libraries(example_module_shared 
    PUBLIC 
    #        example_module_wrapper 
        example_module
)
 
target_compile_options(example_module PUBLIC -msingle-pic-base -fno-inline -fPIE -mno-pic-data-is-text-relative)
target_link_options(example_module PUBLIC -T${PROJECT_SOURCE_DIR}/linker_scripts/dynamic_module.ld -nostartfiles -nodefaultlibs -nostdlib -Wl,--unresolved-symbols=ignore-in-object-files -Wl,--emit-relocs)
