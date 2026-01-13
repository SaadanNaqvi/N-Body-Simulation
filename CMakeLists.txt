cmake_minimum_required(VERSION 3.10)

project(NBodySimulation LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Silence OpenGL deprecation warnings
add_compile_definitions(GL_SILENCE_DEPRECATION)

# Include paths
include_directories(
    include
    dependencies/include
)

# Source files
file(GLOB_RECURSE CPP_SOURCES src/*.cpp)
set(GLAD_SRC dependencies/src/glad.c)

# Create executable
add_executable(myprogram
    ${CPP_SOURCES}
    ${GLAD_SRC}
)

# GLFW library path
target_link_directories(myprogram PRIVATE dependencies/library)

# Link GLFW + macOS frameworks
target_link_libraries(myprogram
    glfw
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
)

