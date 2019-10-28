set (target_name "msos")

add_executable(msos)

target_sources(msos
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(msos
    PUBLIC
        hal
)

include (GenerateBinary)
generate_binary(${target_name} ${vendor})

target_include_directories(msos
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
