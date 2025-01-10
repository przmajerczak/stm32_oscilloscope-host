set(SETTINGS_WINDOW_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/TriggerControls.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SettingsWindow.cpp)

add_subdirectory("controls")

target_sources(stm32_oscilloscope-host PRIVATE ${SETTINGS_WINDOW_SRC})
