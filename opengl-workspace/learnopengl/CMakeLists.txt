# Tutorial 1: Window
add_executable(tut01_window tut01_window.cpp)
target_link_libraries(tut01_window glad glfw ${OPENGL_LIBRARIES})

# Tutrorial 2:Hello Triangle
add_executable(tut02_hello_triagnle tut02_hello_triangle.cpp)
target_link_libraries(tut02_hello_triagnle glad glfw ${OPENGL_LIBRARIES})
