enable_testing()
#set(GTEST_LIBRARY "" CACHE FILEPATH "Location of the Google Test Library")
#set(GTEST_INCLUDE_DIR "" CACHE FILEPATH "Location of the Google Test Include files")
set(GTEST_ROOT_DEFAULT "")
if (DEFINED ENV{GTEST_ROOT})
    set(GTEST_ROOT_DEFAULT "$ENV{GTEST_ROOT}")
endif()
set(GTEST_ROOT "${GTEST_ROOT_DEFAULT}" CACHE PATH "Location of the Google Test Distribution folder containing the lib and include folders")
find_package(GTest)
find_package(ImageMagick COMPONENTS Magick++)
set(ImageMagick_Magick++_DEBUG_LIBRARY "${ImageMagick_Magick++_LIBRARY}" CACHE FILEPATH "Location of the ImageMagick Magick++ Debug library")
# Add support for the Magick++ Debug Library
set(Magick++_LIBRARY optimized "${ImageMagick_Magick++_LIBRARY}" debug "${ImageMagick_Magick++_DEBUG_LIBRARY}")

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${connected_components_SOURCE_DIR}/include)
include_directories(${ImageMagick_Magick++_INCLUDE_DIRS})
add_definitions(-DTEST_DATA="${CMAKE_CURRENT_SOURCE_DIR}/data")
set(SOURCES
    main.cpp
    connected_components_test.cpp)
add_executable (connected_components_test "${SOURCES}")

target_link_libraries(connected_components_test ${GTEST_LIBRARIES} ${Magick++_LIBRARY})
add_custom_command(TARGET connected_components_test PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_CURRENT_SOURCE_DIR}/data $<TARGET_FILE_DIR:connected_components_test>)

