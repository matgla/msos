function(get_device_info mcu mcu_family arch vendor)
    message(STATUS "Configuration of board: KeychainGamer")
    set(${mcu} "STM32F103C8T6" PARENT_SCOPE)
    set(${mcu_family} "STM32F1xx" PARENT_SCOPE)
    set(${arch} "ARM" PARENT_SCOPE)
    set(${vendor} "STM32" PARENT_SCOPE)
endfunction()


function(get_linker_script linker_script linker_scripts_directory)
    set (${linker_script} ${user_boards_path}/Stm32_Black_Pill/linker_script.ld PARENT_SCOPE)
    set (${linker_scripts_directory} ${user_boards_path}/Stm32_Black_Pill PARENT_SCOPE)
endfunction()

function(add_device_hal_library hal_device_library)
    set(${hal_device_library} "nucleo_f103rb_board_library")
    set(hal_device_library ${hal_device_library} PARENT_SCOPE)
    add_library(${hal_device_library} OBJECT)

    target_sources(${hal_device_library} PUBLIC 
        ${user_boards_path}/Stm32_Black_Pill/board.hpp
        ${user_boards_path}/Stm32_Black_Pill/board.cpp
    )

    target_include_directories(${hal_device_library} PUBLIC 
        ${user_boards_path}/Stm32_Black_Pill)

    include(${PROJECT_SOURCE_DIR}/devices/arm/stm32/stm32f1/stm32f103c8t6/configure_stm32f103c8t6.cmake)
    configure_device()

    target_link_libraries(${hal_device_library}
        PUBLIC 
            hal_interface
            hal_devices_arm_stm32f103c8t6
)

endfunction()
