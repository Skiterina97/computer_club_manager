cmake_minimum_required(VERSION 3.10)
project(project)

# Настройки стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. Библиотека логики
# Добавляем и .h и .cpp, чтобы Code::Blocks их видел в дереве проекта
add_library(logic STATIC logic.cpp logic.h)

# 2. Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# Форсируем использование статических библиотек, чтобы не было проблем с DLL
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# 3. Основная программа
add_executable(manager main.cpp)
target_link_libraries(manager logic)

# 4. Тесты
enable_testing()
add_executable(unit_tests tests.cpp)
target_link_options(unit_tests PRIVATE "-static")
# Порядок важен: сначала тесты, потом твоя логика, потом gtest
target_link_libraries(unit_tests 
    logic 
    gtest_main
)

# Это может вызывать ошибку "Error 1", если тесты не могут запуститься при поиске
# Пока закомментируй это, если сборка падает
# include(GoogleTest)
# gtest_discover_tests(unit_tests)