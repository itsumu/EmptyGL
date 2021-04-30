cmake_minimum_required(VERSION 3.8)

project(Glad)

add_library(${PROJECT_NAME}
        src/glad.c
        include/glad/glad.h)

target_include_directories(${PROJECT_NAME} PUBLIC include)