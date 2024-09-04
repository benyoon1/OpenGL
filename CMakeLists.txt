cmake_minimum_required(VERSION 3.27)
project(learnopengl)

set(CMAKE_CXX_STANDARD 23)

set(GL_DEPS "${CMAKE_SOURCE_DIR}/dependencies")

include_directories(${GL_DEPS}/include ${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/configuration ${GL_DEPS}/glad/include)
if (APPLE) 
include_directories(/opt/homebrew/include)
endif()

link_directories(${GL_DEPS}/libs)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)

# Find all .cpp files in the src directory
file(GLOB SRC_FILES src/*.cpp)

# Add executable
add_executable(learnopengl ${SRC_FILES} ${GL_DEPS}/glad/src/glad.c)

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
