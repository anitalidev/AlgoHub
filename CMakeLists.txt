cmake_minimum_required(VERSION 3.22)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

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

