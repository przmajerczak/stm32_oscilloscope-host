add_subdirectory("debug")
add_subdirectory("display")
add_subdirectory("oscilloscope")
add_subdirectory("settingsWindow")
add_subdirectory("sharedData")

target_sources(stm32_oscilloscope-host PRIVATE ${SOURCE})
