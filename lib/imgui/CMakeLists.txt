cmake_minimum_required(VERSION 3.8)

file(GLOB IMGUI_SRC
	src/*.cpp
	include/*.h
)

add_library(IMGUI ${IMGUI_SRC})