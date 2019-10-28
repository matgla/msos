cmake_minimum_required(VERSION 3.9)

project(msos NONE)

include(cmake/GitModules.cmake)

set (user_boards_path ${PROJECT_SOURCE_DIR}/boards CACHE STRING "Path to boards provided from user" FORCE)
add_definitions("-DGSL_TERMINATE_ON_CONTRACT_VIOLATION=1 -DGSL_MSVC_USE_STL_NOEXCEPTION_WORKAROUND=1")

fetch_module_via_branch(lib/hal ${PROJECT_SOURCE_DIR}/lib/hal ${CMAKE_CURRENT_SOURCE_DIR} "master")
#fetch_module_via_branch(lib/msgui ${PROJECT_SOURCE_DIR}/lib/msgui ${CMAKE_CURRENT_SOURCE_DIR} "master")

set(CMAKE_EXECUTABLE_SUFFIX "${CMAKE_EXECUTABLE_SUFFIX}")

include (SetCompilationSystem)
set_compilation_target()

enable_language(ASM C CXX)

add_subdirectory(include/msos/drivers)
add_subdirectory(src)
