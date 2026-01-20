cmake_minimum_required(VERSION 3.10)

project(NBodySimulation LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(GL_SILENCE_DEPRECATION)

# Include paths
include_directories(
    include
    dependencies/include
)

set(GLAD_SRC dependencies/src/glad.c)

# Absolute path to GLFW
set(GLFW_LIB "${CMAKE_SOURCE_DIR}/dependencies/library/libglfw.3.4.dylib")

file(GLOB_RECURSE CPP_SOURCES src/*.cpp)


add_executable(simulation
    ${CPP_SOURCES}
    ${GLAD_SRC}
)

target_link_libraries(simulation
    ${GLFW_LIB}
    "-framework OpenGL"
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
)



