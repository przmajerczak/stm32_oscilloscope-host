set(MEASUREMENTS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/VerticalMeasurements.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HorizontalMeasurements.cpp)

target_sources(stm32_oscilloscope-host PRIVATE ${MEASUREMENTS_SRC})
