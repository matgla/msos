set (target_name "msos")

add_executable(${target_name})

target_sources(${target_name}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(${target_name}
    PUBLIC
        hal
        msos_drivers
        msgui
)

include (GenerateBinary)
generate_binary(${target_name} ${vendor})

target_include_directories(${target_name}
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)
