cmake_minimum_required(VERSION 3.8)

project(EmptyGL LANGUAGES CXX)

# Find third party packages
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Add executables
add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include
		deps/cxxopts
		${CMAKE_CURRENT_SOURCE_DIR}/deps/stb)

# Link libraries
add_subdirectory(deps/glad)
add_subdirectory(src)

set(ALL_LIBS
		${OPENGL_LIBRARY}
		Glad
		glfw
		SelfLibs)

target_link_libraries(${PROJECT_NAME} PRIVATE ${ALL_LIBS})
