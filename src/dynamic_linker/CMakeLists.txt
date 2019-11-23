add_library(msos_dynamic_linker)

set (library_includes ${PROJECT_SOURCE_DIR}/include/msos/dynamic_linker)

target_sources(msos_dynamic_linker 
    PUBLIC 
        ${library_includes}/dynamic_linker.hpp
        ${library_includes}/symbol.hpp
        ${library_includes}/relocation.hpp
        ${library_includes}/module.hpp 
        ${library_includes}/environment.hpp

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/dynamic_linker.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/symbol.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/relocation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/module.cpp
)

target_include_directories(msos_dynamic_linker
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(msos_dynamic_linker
    PUBLIC 
        hal
)

