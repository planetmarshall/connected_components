message("${connected_components_SOURCE_DIR}/include")
set(HEADER_DIR "${connected_components_SOURCE_DIR}/include")
include_directories(${HEADER_DIR})
set(HEADERS
    "${HEADER_DIR}/connected_components.hpp"
    )

add_executable (connected_components main.cpp "${HEADERS}")
