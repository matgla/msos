add_executable(host)

target_sources(host
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(host
    PUBLIC
        hal
        msos_dynamic_linker
)

include (GenerateBinary)
generate_binary(host ${vendor})

target_include_directories(host 
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)

set_target_properties(host 
    PROPERTIES
        SUFFIX ".elf"
)


