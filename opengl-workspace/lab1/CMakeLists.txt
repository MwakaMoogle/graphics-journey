# Exercise 1: Basic
add_executable(lab1_basic basic.cpp)
target_link_libraries(lab1_basic core_engine)

# Exercise 2: Basic Wrapper
add_executable(lab1_basic_wrapper basic_wrapper.cpp)
target_link_libraries(lab1_basic_wrapper core_engine)

# Exercise 3: Vertex Attributes
add_executable(lab1_vertex_attribs vertex_attribs.cpp)
target_link_libraries(lab1_vertex_attribs core_engine)
