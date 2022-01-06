cmake_minimum_required(VERSION 3.5)

project(Game2048 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Game2048 test.cpp game2048.cpp game2048.hpp)
