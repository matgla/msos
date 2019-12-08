function (register_system_test_binary name)
    add_custom_target(
        ${name}_generate_test_image
        COMMAND cat ${HOST_BINARY_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${name}.bin >
        ${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin 
        DEPENDS host ${name} 
        VERBATIM 
    )

    set (run_st_deps "${run_st_deps}" ${name}_generate_test_image CACHE INTERNAL "")

    list(FIND BINARIES_FOR_ST "${name}:${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin" test_in_list)

    if (${test_in_list} EQUAL -1)
        message (STATUS "Adding binary for test environment: ${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin")
        set(BINARIES_FOR_ST "${BINARIES_FOR_ST}" "${name}:${CMAKE_CURRENT_BINARY_DIR}/test_binary.bin" CACHE INTERNAL "")
    endif()


endfunction()
