cmake_minimum_required(VERSION 3.28)
project(fingers_detection)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(fingers_detection main.cpp)
target_link_libraries(fingers_detection ${OpenCV_LIBS})
