set(CONTROLS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/VerticalBoundControls.cpp)

target_sources(stm32_oscilloscope-host PRIVATE ${CONTROLS_SRC})
