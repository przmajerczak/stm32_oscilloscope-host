set(SOURCE 
    ${SOURCE}
    ${CMAKE_CURRENT_SOURCE_DIR}/DataRetriever.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DataAnalyzer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DisplayHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SettingsWindow.cpp)

target_sources(stm32_oscilloscope-host PRIVATE ${SOURCE})
