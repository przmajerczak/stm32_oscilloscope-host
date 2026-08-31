set(OSCILLOSCOPE_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/DataAnalyzer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DataRetriever.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Oscilloscope.cpp)

target_sources(stm32_oscilloscope-host PRIVATE ${OSCILLOSCOPE_SRC})
