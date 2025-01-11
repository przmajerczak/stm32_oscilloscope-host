set(SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/DataRetriever.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DataAnalyzer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DisplayHelper.cpp)

add_subdirectory("settingsWindow")
add_subdirectory("sharedData")

target_sources(stm32_oscilloscope-host PRIVATE ${SOURCE})
