add_library(simple_module_library OBJECT)

target_sources(simple_module_library
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(simple_module_library
    PRIVATE
        module_flags
        msos_utils
)

add_module(simple_module simple_module_library)

include (TestUtils)
register_system_test_binary(simple_module)

