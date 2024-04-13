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
add_executable (suffix_array "suffix_array.cpp")
add_executable (random_tree "random_tree.cpp")
add_executable (prim "prim.cpp")

enable_testing()
add_test(TestHungarian hungarian)
add_test(TestSuffixArray suffix_array)
add_test(RandomTree random_tree)
add_test(TestPrim prim)
