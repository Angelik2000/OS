cmake_minimum_required(VERSION 3.10)
project(ThreadLab2)

set(CMAKE_CXX_STANDARD 17)

add_executable(ThreadLab2
    src/main.cpp
    src/core.cpp
    src/min_max_wrapper.cpp
    src/average_wrapper.cpp
    src/thread_utils.cpp
)