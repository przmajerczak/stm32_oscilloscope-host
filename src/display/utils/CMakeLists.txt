set(DISPLAY_UTILS_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/LineDrawer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TextPrinter.cpp)

target_sources(stm32_oscilloscope-host PRIVATE ${DISPLAY_UTILS_SRC})
