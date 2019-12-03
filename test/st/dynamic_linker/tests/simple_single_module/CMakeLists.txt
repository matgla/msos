add_library(simple_module OBJECT)

target_sources(simple_module
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(simple_module
    PRIVATE 
        module_flags
)

add_wrapper(simple_module)


