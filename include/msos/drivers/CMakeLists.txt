set (library_name msos_drivers)

add_library(${library_name} INTERFACE)

add_subdirectory(displays)
target_link_libraries(${library_name}
    INTERFACE
        msos_drivers_displays)
