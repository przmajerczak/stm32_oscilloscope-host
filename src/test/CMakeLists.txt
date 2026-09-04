set(TEST_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/DummyOutputGenerator.cpp)

target_sources(stm32_oscilloscope-host PRIVATE ${TEST_SRC})
