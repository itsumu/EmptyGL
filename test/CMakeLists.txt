cmake_minimum_required(VERSION 3.8)

project(Test LANGUAGES CXX)

find_package(Eigen3 CONFIG REQUIRED)

# Eigen test
add_executable(TestEigen test.cpp)
target_include_directories(TestEigen PRIVATE ${EIGEN3_INCLUDE_DIR})

