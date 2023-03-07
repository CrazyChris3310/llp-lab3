cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER "gcc")

project(dbms LANGUAGES C)

file(GLOB_RECURSE C_FILES_LIB CONFIGURE_DEPENDS src/*.c)
file(GLOB_RECURSE H_FILES_LIB CONFIGURE_DEPENDS src/*.h)

add_compile_options(
  -pedantic-errors
  -Wall -Werror
  -pedantic-errors
  -g3
  -O0
)

add_library(${PROJECT_NAME} STATIC ${C_FILES_LIB} ${H_FILES_LIB})
target_include_directories(${PROJECT_NAME} PUBLIC src/main)