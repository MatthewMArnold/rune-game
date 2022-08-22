# cmake needs this line
cmake_minimum_required(VERSION 3.1)

# Define project name
project(rune_game)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    src/settings.cpp
    src/main.cpp
    src/plate_finder.cpp
    src/binarize_image.cpp
    src/petal_finder.cpp
    src/filters.cpp
    src/plate_selector_strip_finder.cpp
    src/center_finder.cpp
    src/target_plate_selector.cpp
    src/center_roi_finder.cpp
    src/video_dumper.cpp)

include_directories(${PROJECT_SOURCE_DIR}/src)

# ADD_LIBRARY(rune_game SHARED ${SOURCES})
add_executable(rune_game ${SOURCES})

# Link your application with OpenCV libraries
target_link_libraries(rune_game PRIVATE ${OpenCV_LIBS})
