cmake_minimum_required(VERSION 3.28)
project(learnopengl)

set(CMAKE_CXX_STANDARD 23)

set(GLFW_DIR "/home/byoon/CLionProjects/learnopengl")

include_directories(${GLFW_DIR}/include)

link_directories(${GLFW_DIR}/libs)

add_executable(learnopengl src/main.cpp
        src/glad.c)

target_link_libraries(learnopengl
        ${GLFW_DIR}/libs/libglfw3.a
        GL
        X11
        pthread
        Xrandr
        Xi
        dl
)
