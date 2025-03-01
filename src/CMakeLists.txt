set(SOURCE 
    ${CMAKE_CURRENT_SOURCE_DIR}/DataRetriever.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DataAnalyzer.cpp)

add_subdirectory("debug")
add_subdirectory("display")
add_subdirectory("settingsWindow")
add_subdirectory("sharedData")

target_sources(stm32_oscilloscope-host PRIVATE ${SOURCE})
