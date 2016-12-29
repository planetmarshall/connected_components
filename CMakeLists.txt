cmake_minimum_required (VERSION 2.6)
project (connected_components)

set (connected_components_VERSION_MAJOR 0)
set (connected_components_VERSION_MINOR 1)

configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
    )

include_directories("${PROJECT_BINARY_DIR}")
add_subdirectory(src)
add_subdirectory(test)
