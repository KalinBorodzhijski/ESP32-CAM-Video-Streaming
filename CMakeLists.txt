cmake_minimum_required(VERSION 3.16.0)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(esp32_project)

set(EXTRA_COMPONENT_DIRS src)

# Include the main source files and directories
idf_build_component(${CMAKE_CURRENT_LIST_DIR}/src)
