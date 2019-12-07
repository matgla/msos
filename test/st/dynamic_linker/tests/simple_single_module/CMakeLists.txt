add_library(simple_module OBJECT)

target_sources(simple_module
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries(simple_module
    PRIVATE 
        module_flags
)

add_wrapper(simple_module)

add_custom_target(
    concat_binaries 
    DEPENDS host_resize ${CMAKE_CURRENT_BINARY_DIR}/simple_module_generate_bin.stamp
    COMMAND cat ${HOST_BINARY_FILE} ${CMAKE_CURRENT_BINARY_DIR}/simple_module.bin >
    ${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin 
    VERBATIM 
)

add_dependencies(concat_binaries simple_module)

list(FIND BINARIES_FOR_ST "simple_module:${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin" test_in_list)

if (${test_in_list} EQUAL -1)
    message (STATUS "Adding binary for test environment: ${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin")
    set(BINARIES_FOR_ST "${BINARIES_FOR_ST}" "simple_module:${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin" CACHE INTERNAL "")
endif()

add_dependencies(generate_config_file concat_binaries)

