add_library(example_module_library OBJECT)

target_sources(example_module_library
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

include(ModuleWrapperGenerator)

add_module_flags_target()
target_link_libraries(example_module_library PUBLIC module_flags)

add_module(example_module example_module_library)

