function(get_device_info mcu mcu_family arch vendor)
    message(STATUS "Configuration of board: KeychainGamer")
    set(${mcu} "STM32F103RBT6" PARENT_SCOPE)
    set(${mcu_family} "STM32F1xx" PARENT_SCOPE)
    set(${arch} "ARM" PARENT_SCOPE)
    set(${vendor} "STM32" PARENT_SCOPE)
endfunction()


function(get_linker_script linker_script linker_scripts_directory)
    set (${linker_script} ${user_boards_path}/NUCLEO-F103RB/linker_script.ld PARENT_SCOPE)
    set (${linker_scripts_directory} ${user_boards_path}/NUCLEO-F103RB PARENT_SCOPE)
endfunction()

function(add_device_hal_library hal_device_library)
    set(${hal_device_library} "nucleo_f103rb_board_library")
    set(hal_device_library ${hal_device_library} PARENT_SCOPE)
    add_library(${hal_device_library} OBJECT)

    target_sources(${hal_device_library} PUBLIC
        ${user_boards_path}/NUCLEO-F103RB/board.hpp
    )

    target_include_directories(${hal_device_library} PUBLIC
        ${user_boards_path}/NUCLEO-F103RB)

    include(${PROJECT_SOURCE_DIR}/devices/arm/stm32/stm32f1/stm32f103rbt6/configure_stm32f103rbt6.cmake)
    configure_device()

    target_link_libraries(${hal_device_library}
        PUBLIC 
            hal_interface
            hal_devices_arm_stm32f103rbt6
)

endfunction()
