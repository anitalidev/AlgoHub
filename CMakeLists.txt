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

# Recursively find Topic solution files under Topics/**/Solution.cpp and create one executable per file.
# Create stable, unique target names by prefixing with the containing folder name and replacing unsafe chars with '_'.
file(GLOB_RECURSE TOPIC_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Topics/**/Solution.cpp")
foreach(T_FILE ${TOPIC_SOURCES})
    # get path relative to project root
    file(RELATIVE_PATH REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${T_FILE}")
    # get the file base name (without extension) and the immediate parent directory name
    get_filename_component(FILE_BASE "${REL_PATH}" NAME_WE)
    get_filename_component(PARENT_DIR "${REL_PATH}" DIRECTORY)
    get_filename_component(PARENT_NAME "${PARENT_DIR}" NAME)
    # build target name as <parent>_<filebase> (e.g. "Kth-Largest-Element-in-an-Array_Solution")
    set(TARGET_BASE "${PARENT_NAME}_${FILE_BASE}")
    # sanitize: replace '/', '-', '.' with '_'
    string(REPLACE "/" "_" TARGET_BASE "${TARGET_BASE}")
    string(REPLACE "-" "_" TARGET_BASE "${TARGET_BASE}")
    string(REPLACE "." "_" TARGET_BASE "${TARGET_BASE}")
    # final target name
    set(TARGET_NAME "${TARGET_BASE}")
    add_executable(${TARGET_NAME} ${T_FILE})
endforeach()
