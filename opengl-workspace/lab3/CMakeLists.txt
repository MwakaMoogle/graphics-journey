# Exercise 1: Start
add_executable(lab3_start lab3start.cpp)
target_link_libraries(lab3_start core_engine)

target_include_directories(lab3_start PRIVATE ${CMAKE_SOURCE_DIR}/src)

