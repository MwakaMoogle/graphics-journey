cmake_minimum_required(VERSION 3.14)
project(OpenGL_Workspace)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Force all compiled executables to output to the root build/ directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(include)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

# --- Auto-Download GLFW ---
include(FetchContent)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
FetchContent_MakeAvailable(glfw)

# --- OS-Specific OpenGL ---
if (APPLE)
    find_library(OPENGL_LIBRARIES OpenGL REQUIRED)
elseif (UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    set(OPENGL_LIBRARIES OpenGL::GL)
endif()

# --- Compile Shared Code ---
# Bundle glad and the uni's wrapper class into one library
add_library(core_engine src/glad.c src/wrapper_glfw.cpp)

# Ensure the core engine knows where its headers are
target_include_directories(core_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link the core engine to OpenGL and GLFW so the wrapper functions work
target_link_libraries(core_engine glfw ${OPENGL_LIBRARIES})

# --- Add Sub-Projects ---
#add_subdirectory(learnopengl)
add_subdirectory(lab1)
