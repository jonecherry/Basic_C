cmake_minimum_required(VERSION 3.6)
project(MachineLearning2017)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(MachineLearning2017 ${SOURCE_FILES})