cmake_minimum_required(VERSION 3.14)
Project(MAZE LANGUAGES CXX)

add_library(maze-lib INTERFACE)

target_include_directories(maze-lib
    INTERFACE
        include/
)

target_compile_features(maze-lib
    INTERFACE
        cxx_std_20
)

target_compile_options(maze-lib
    INTERFACE
        -Wall
        -Wextra
        -Wpedantic
)

# Tests
add_executable(maze-test EXCLUDE_FROM_ALL tests/test.main.cpp)
target_link_libraries(maze-test PUBLIC maze-lib)
