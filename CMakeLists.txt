cmake_minimum_required(VERSION 2.8)
project(solve-sudoku-with-vision)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(include)

file(GLOB SOURCES "src/*.cpp")
add_executable(SudokuSolver ${SOURCES})
target_link_libraries(SudokuSolver ${OpenCV_LIBS})