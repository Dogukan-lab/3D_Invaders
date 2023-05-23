cmake_minimum_required(VERSION 3.22)
add_library(tigl tigl.cpp)
target_include_directories(tigl PRIVATE include)