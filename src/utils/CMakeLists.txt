# This should be shared library, but I have to add support for dependencies and fs

add_library(msos_utils INTERFACE)

set (library_includes ${PROJECT_SOURCE_DIR}/include/msos/utils)

target_sources(msos_utils
    INTERFACE
        ${library_includes}/string.hpp
)

target_include_directories(msos_utils
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)
