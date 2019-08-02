cmake_minimum_required(VERSION 3.10.2)

project (SudokuSolver)

# add the headers in ./include to the project
include_directories(include)

add_subdirectory (src)

enable_testing ()
add_subdirectory (testing)

SET(CMAKE_CXX_FLAGS "-std=c++0x")
