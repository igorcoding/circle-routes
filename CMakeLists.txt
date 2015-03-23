cmake_minimum_required(VERSION 3.1)
project(circle_routes)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include/)

add_subdirectory(src/)
add_subdirectory(test/)
add_subdirectory(server/)