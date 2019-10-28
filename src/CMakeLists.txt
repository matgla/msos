set (target_name "msos")

add_executable(msos)

target_sources(msos
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_library(aaa INTERFACE)
target_link_libraries(aaa INTERFACE hal_stm32_common)

target_link_libraries(msos
    PUBLIC
        hal
        #msos_drivers
        #        msgui
        aaa
)

include (GenerateBinary)
generate_binary(${target_name} ${vendor})

target_include_directories(msos
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
