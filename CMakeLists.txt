cmake_minimum_required(VERSION 3.10)

project(RenderingGeometryProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

include_directories(
    include
    external/glad/include
)

set(SOURCES
    src/main.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/Mesh.cpp
    src/BezierCurve.cpp
    src/BezierSurface.cpp
    src/Renderer.cpp
    src/Utils.cpp
    external/glad/src/gl.c
)

add_executable(RenderingGeometryProject ${SOURCES})

target_link_libraries(RenderingGeometryProject
    OpenGL::GL
    glfw
)
