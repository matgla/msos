set (library_name msos_drivers_displays)

add_library(${library_name} INTERFACE)

target_sources(${library_name}
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/ssd1306/ssd1306.hpp
)

