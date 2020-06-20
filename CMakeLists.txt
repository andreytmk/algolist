# CMakeList.txt : CMake project for algolist, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("algolist")

# Add source to this project's executable.
add_executable (hungarian "hungarian.cpp")

enable_testing()
add_test(TestHungarian hungarian)
