cmake_minimum_required(VERSION 3.10)
project(OrbitalSimulation)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenGL_GL_PREFERENCE GLVND)

# Find necessary system libraries
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# Add your source files (including glad.c)
add_executable(orbital_engine 
    src/main.cpp 
    src/glad.c
)

# Link the libraries to your executable
target_link_libraries(orbital_engine 
    glfw 
    OpenGL::GL 
    dl
)

target_include_directories(orbital_engine PRIVATE ${CMAKE_SOURCE_DIR}/include)
