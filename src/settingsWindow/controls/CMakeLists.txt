set(CONTROLS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/DebugDataControls.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AveragingControls.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/MeasurementsControls.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TriggerControls.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HorizontalResolutionControls.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/VerticalBoundControls.cpp)

add_subdirectory("measurements")

target_sources(stm32_oscilloscope-host PRIVATE ${CONTROLS_SRC})
