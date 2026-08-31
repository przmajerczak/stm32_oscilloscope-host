set(OSCILLOSCOPE_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/DataAnalyzer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Oscilloscope.cpp)

add_subdirectory("dataAcquisition")

target_sources(stm32_oscilloscope-host PRIVATE ${OSCILLOSCOPE_SRC})
