cmake_minimum_required(VERSION 3.2)
project(TaskImGui LANGUAGES CXX)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_GENERATOR "Ninja" CACHE STRING "" FORCE)
set(CMAKE_CXX_COMPILER g++)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../lib lib)
add_executable(TaskImGui main.cpp)

# Linking to the imgui library
target_link_libraries(TaskImGui PRIVATE imgui)

# Add directory of headers
target_include_directories(TaskImGui PRIVATE ../headers)
target_link_libraries(TaskImGui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../libglfw3.a)

# Finding OpenGL and set variable OPENGL_INCLUDE_DIR with directory of OpenGL
find_package(OpenGL REQUIRED)

target_include_directories(TaskImGui PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(TaskImGui PRIVATE OpenGL::GL)
