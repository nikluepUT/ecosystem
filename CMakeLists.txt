cmake_minimum_required(VERSION 3.15)
project(ecosystem)

set(CMAKE_CXX_STANDARD 14)
# added -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp")

add_executable(ecosystem main.cpp Field.cpp Field.h World.h World.cpp)
