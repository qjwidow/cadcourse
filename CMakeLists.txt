cmake_minimum_required(VERSION 3.16)
project(CADCourseCode_cpp LANGUAGES CXX)

find_package(glfw3 REQUIRED) # glfw
find_package(OpenGL REQUIRED) # opengl
find_package(GLEW REQUIRED) # glew
find_package(GLUT REQUIRED) # GLUT

include_directories(/usr/local/include)
include_directories(
    src/HalfEdgeDS
    src/EulerOp
    src/Sweep
    src/Draw
)

add_executable(CADCourseCode_cpp
    src/main.cpp
    src/HalfEdgeDS/HalfEdgeDS.cpp
    src/EulerOp/EulerOp.cpp
    src/Sweep/Sweep.cpp
    src/Draw/Draw.cpp
)

target_link_libraries(
    CADCourseCode_cpp
    glfw
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLUT_LIBRARIES}
)