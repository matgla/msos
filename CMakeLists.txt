cmake_minimum_required(VERSION 3.9)

project(msos CXX C ASM)

include(cmake/GitModules.cmake)
fetch_module_via_branch(lib/hal ${PROJECT_SOURCE_DIR}/lib/hal ${CMAKE_CURRENT_SOURCE_DIR} "master")

add_library(msos INTERFACE)

target_link_libraries(msos
    INTERFACE
        hal
)
