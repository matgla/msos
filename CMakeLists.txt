cmake_minimum_required(VERSION 3.9)

project(msos NONE)

include(cmake/GitModules.cmake)

fetch_module_via_branch(lib/hal ${PROJECT_SOURCE_DIR}/lib/hal ${CMAKE_CURRENT_SOURCE_DIR} "master")

set(CMAKE_EXECUTABLE_SUFFIX "${CMAKE_EXECUTABLE_SUFFIX}")

include (SetCompilationSystem)
set_compilation_target()

enable_language(ASM C CXX)

add_library(msos INTERFACE)

target_link_libraries(msos
    INTERFACE
        hal
)
