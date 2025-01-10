set(SETTINGS_WINDOW_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/VerticalBoundControls.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SettingsWindow.cpp)

target_sources(stm32_oscilloscope-host PRIVATE ${SETTINGS_WINDOW_SRC})
