add_library(simple_module_library OBJECT)

target_sources(simple_module_library
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(simple_module_library
    PRIVATE 
        module_flags
)

add_module(simple_module simple_module_library)

add_dependencies(simple_module host)
add_custom_target(
    simple_module_generate_test_image
    COMMAND cat ${HOST_BINARY_FILE} ${CMAKE_CURRENT_BINARY_DIR}/simple_module.bin >
    ${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin 
    DEPENDS host simple_module
    VERBATIM 
)

set (run_st_deps "${run_st_deps}" simple_module_generate_test_image CACHE INTERNAL "")

list(FIND BINARIES_FOR_ST "simple_module:${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin" test_in_list)

if (${test_in_list} EQUAL -1)
    message (STATUS "Adding binary for test environment: ${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin")
    set(BINARIES_FOR_ST "${BINARIES_FOR_ST}" "simple_module:${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin" CACHE INTERNAL "")
endif()


