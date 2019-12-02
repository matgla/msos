add_library(example_module OBJECT)

target_sources(example_module
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

include(ModuleWrapperGenerator)

add_module_flags_target()
target_link_libraries(example_module PUBLIC module_flags)

add_wrapper(example_module)

