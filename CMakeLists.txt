cmake_minimum_required(VERSION 3.3)
project(RegularExpressions)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lgtest -pthread")

add_executable(RegularExpressions main.cpp)
add_executable(Tests tests.cpp)
add_library(Solution Solution.cpp Solution.h)
target_link_libraries(Tests gtest Solution)
target_link_libraries(RegularExpressions Solution)