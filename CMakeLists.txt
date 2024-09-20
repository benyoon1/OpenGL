cmake_minimum_required(VERSION 3.10)
project(opengl)

set(CMAKE_CXX_STANDARD 17)

# Find the Assimp package
find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)

if(glfw3_FOUND)
message(STATUS "GLFW found")
else()
message(FATAL_ERROR "GLFW not found")
endif()

if(assimp_FOUND)
message(STATUS "Assimp found") 
else()
message(FATAL_ERROR "Assimp not found")
endif()

set(GL_DEPS "${CMAKE_SOURCE_DIR}/dependencies")

include_directories(${GL_DEPS}/glad/include 
                    ${CMAKE_SOURCE_DIR}/include 
                    ${CMAKE_BINARY_DIR}/configuration 
                    ${ASSIMP_INCLUDE_DIRS})

configure_file(configuration/root_directory.h.in configuration/root_directory.h)

# Find all .cpp files in the src directory
file(GLOB SRC_FILES src/*.cpp)

# Add executable
add_executable(opengl ${SRC_FILES} ${GL_DEPS}/glad/src/glad.c)

if(APPLE)
        target_link_libraries(opengl
                glfw
                assimp::assimp
                "-framework OpenGL"
                "-framework Cocoa"
                "-framework IOKit"
                "-framework CoreVideo"
)
elseif(UNIX AND NOT APPLE)
        target_link_libraries(opengl
                glfw
                assimp::assimp
                GL
                X11
                pthread
                Xrandr
                Xi
                dl
        )
endif()

