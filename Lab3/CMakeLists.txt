cmake_minimum_required(VERSION 3.14)

project(SyncLab VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX /D_CRT_SECURE_NO_WARNINGS)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(core_lib
    src/Orchestrator.cpp
    src/Orchestrator.h
    src/Marker.cpp
    src/Marker.h
    src/SharedData.h
    src/Utils.h
)

add_executable(SyncLabApp src/main.cpp)
target_link_libraries(SyncLabApp PRIVATE core_lib)

enable_testing()
add_executable(UnitTests tests/test_main.cpp)
target_link_libraries(UnitTests PRIVATE core_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(UnitTests)