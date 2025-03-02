set(DEBUG_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/StatsForTimemarker.cpp)

target_sources(stm32_oscilloscope-host PRIVATE ${DEBUG_SRC})
