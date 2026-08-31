set(DATA_ACQUISITION_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/DataRetriever.cpp)

target_sources(stm32_oscilloscope-host PRIVATE ${DATA_ACQUISITION_SRC})
