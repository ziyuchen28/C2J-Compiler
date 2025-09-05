 
cmake_minimum_required(VERSION 3.15)
project(c2j LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# source
add_library(c2jlib
  src/arena.cpp
)
target_include_directories(c2jlib
  PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
add_executable(c2j 
    src/main.cpp
)
target_link_libraries(c2j PRIVATE c2jlib)


# tests
add_executable(c2jtest 
    tests/test_main.cpp
    tests/arena_tests.cpp
)
target_include_directories(c2jtest
  PRIVATE  ${CMAKE_CURRENT_SOURCE_DIR}/include
           ${CMAKE_CURRENT_SOURCE_DIR}/tests
)
target_link_libraries(c2jtest PRIVATE c2jlib)



