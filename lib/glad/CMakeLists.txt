cmake_minimum_required(VERSION 3.22)

file(GLOB GLAD_SRC
        gl.c
        include/*.h)

add_library(glad STATIC ${GLAD_SRC})
