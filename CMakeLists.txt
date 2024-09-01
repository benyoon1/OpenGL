cmake_minimum_required(VERSION 3.27)
project(learnopengl)

set(CMAKE_CXX_STANDARD 23)

set(GL_DEPS "${CMAKE_SOURCE_DIR}/dependencies")

include_directories(${GL_DEPS}/include ${CMAKE_SOURCE_DIR}/include /opt/homebrew/include ${GL_DEPS}/glad/include)

link_directories(${GL_DEPS}/libs)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

add_executable(learnopengl src/main.cpp
                ${GL_DEPS}/glad/src/glad.c
                src/stb_image.cpp)

if(APPLE)
        target_link_libraries(learnopengl
                ${GL_DEPS}/libs/libglfw3.a
                "-framework OpenGL"
                "-framework Cocoa"
                "-framework IOKit"
                "-framework CoreVideo"
)
elseif(UNIX AND NOT APPLE)
        target_link_libraries(learnopengl
                ${GL_DEPS}/libs/libglfw3.a
                GL
                X11
                pthread
                Xrandr
                Xi
                dl
        )
endif()
