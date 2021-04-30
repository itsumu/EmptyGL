cmake_minimum_required(VERSION 3.8)

find_package(Eigen3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_library(SelfLibs
        geometry.cpp
        scene.cpp
        shader.cpp
        camera.cpp
        mesh.cpp)

target_link_libraries(SelfLibs PUBLIC
        Glad
        assimp::assimp)

target_include_directories(SelfLibs PUBLIC
        ../include
        ../deps/stb
        ../deps/glad/include
        ${OPENMESH_INCLUDE_DIRS}
        ${EIGEN3_INCLUDE_DIR})
