set(SHARED_DATA_SRC
    #${CMAKE_CURRENT_SOURCE_DIR}/FIRST_FILE.cpp
    )

target_sources(stm32_oscilloscope-host PRIVATE ${SHARED_DATA_SRC})
