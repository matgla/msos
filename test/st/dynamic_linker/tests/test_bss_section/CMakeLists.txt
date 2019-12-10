add_library(test_bss_section_library OBJECT)

target_compile_options(test_bss_section_library
    PUBLIC 
    $<$<CONFIG:DEBUG>:-Og -g>
    $<$<CONFIG:RELEASE>:-Os>
) 


target_sources(test_bss_section_library
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(test_bss_section_library
    PUBLIC
        module_flags
        msos_utils
)

add_module(test_bss_section test_bss_section_library)

include (TestUtils)
register_system_test_binary(test_bss_section)

