# CMakeList.txt : CMake project for algolist, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

project ("algolist")

# Add source to this project's executable.
add_executable (hungarian "hungarian.cpp")

enable_testing()
add_test(TestHungarian hungarian)
