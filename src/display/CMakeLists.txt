set(DISPLAY_SRC ${CMAKE_CURRENT_SOURCE_DIR}/DisplayHelper.cpp)

add_subdirectory("utils")

target_sources(stm32_oscilloscope-host PRIVATE ${DISPLAY_SRC})
