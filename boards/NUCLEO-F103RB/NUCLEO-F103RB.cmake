function(get_device_info mcu mcu_family arch vendor)
    message(STATUS "Configuration of board: KeychainGamer")
    set(${mcu} "STM32F103RBT6" PARENT_SCOPE)
    set(${mcu_family} "STM32F1xx" PARENT_SCOPE)
    set(${arch} "ARM" PARENT_SCOPE)
    set(${vendor} "STM32" PARENT_SCOPE)
endfunction()

function(add_device_hal_library hal_device_library)
    set(${hal_device_library} "board_library")
    set(hal_device_library ${hal_device_library} PARENT_SCOPE)
    add_library(${hal_device_library} INTERFACE)

    target_sources(${hal_device_library} INTERFACE
        ${user_boards_path}/NUCLEO-F103RB/board.hpp
    )

    target_include_directories(${hal_device_library} INTERFACE
        ${user_boards_path}/F103RB)

    include(${PROJECT_SOURCE_DIR}/devices/arm/stm32/stm32f1/stm32f103rbt6/configure_stm32f103rbt6.cmake)

    target_link_libraries(${hal_device_library}
        INTERFACE
            hal_interface
            hal_devices_avr_attiny85
    )

endfunction()