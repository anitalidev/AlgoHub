cmake_minimum_required(VERSION 3.22)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

# Auto-add individual executables for any q*.cpp files placed in their own folder (e.g. 133/q133.cpp).
# Uses CONFIGURE_DEPENDS so CMake will re-run when files are added/removed (requires CMake >= 3.12).
# This matches files one level down: <project-root>/*/q*.cpp
file(GLOB Q_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*/q*.cpp")
foreach(Q_FILE ${Q_SOURCES})
    get_filename_component(Q_NAME ${Q_FILE} NAME_WE)
    # Create an executable named after the file (e.g. q133 -> q133)
    add_executable(${Q_NAME} ${Q_FILE})
endforeach()
