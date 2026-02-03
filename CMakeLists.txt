cmake_minimum_required(VERSION 3.10)
project(project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(logic STATIC logic.cpp logic.h)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(manager main.cpp)
target_link_libraries(manager logic)

enable_testing()
add_executable(unit_tests tests.cpp)
target_link_options(unit_tests PRIVATE "-static")
target_link_libraries(unit_tests 
    logic 
    gtest_main
)