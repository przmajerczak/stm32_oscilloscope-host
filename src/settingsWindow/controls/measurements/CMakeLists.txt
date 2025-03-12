set(MEASUREMENTS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/VerticalMeasurements.cpp)

target_sources(stm32_oscilloscope-host PRIVATE ${MEASUREMENTS_SRC})
