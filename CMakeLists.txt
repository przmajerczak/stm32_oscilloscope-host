cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("stm32_oscilloscope-host")

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(FREETYPE REQUIRED freetype2)

include_directories(${GTK_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

set(GLUT_INCLUDE_DIR /usr/include/)
set(GLUT_LIBRARY /usr/lib/x86_64-linux-gnu/libglut.so)

set(MAIN_SRC ${CMAKE_CURRENT_SOURCE_DIR}/stm32_oscilloscope-host.cpp)

add_executable(stm32_oscilloscope-host ${MAIN_SRC})

target_include_directories(stm32_oscilloscope-host PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(stm32_oscilloscope-host ${GTK_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${FREETYPE_LIBRARIES})

add_subdirectory("src")

target_sources(stm32_oscilloscope-host PRIVATE ${MAIN_SRC})
