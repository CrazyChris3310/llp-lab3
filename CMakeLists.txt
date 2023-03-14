cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER "g++")

set(CLIENT_NAME lab3-client)
set(SERVER_NAME lab3-server)

project(lab3-client LANGUAGES CXX)

file(GLOB_RECURSE C_FILES_CLIENT CONFIGURE_DEPENDS client/*.cpp)
file(GLOB_RECURSE H_FILES_CLIENT CONFIGURE_DEPENDS client/*.h)

file(GLOB_RECURSE C_FILES_COMMON CONFIGURE_DEPENDS common/*.cxx)
file(GLOB_RECURSE H_FILES_COMMON CONFIGURE_DEPENDS common/*.hxx)

add_compile_options(
  -pedantic-errors
  -Wall -Werror
  -g3
  -O0
)

add_executable(${CLIENT_NAME} ${C_FILES_CLIENT} ${H_FILES_CLIENT} ${C_FILES_COMMON} ${H_FILES_COMMON})
target_include_directories(${CLIENT_NAME} PUBLIC client parser_lib common)
add_subdirectory(parser_lib)
target_link_libraries(${CLIENT_NAME} xerces-c parser_lib)

file(GLOB_RECURSE C_FILES_SERVER CONFIGURE_DEPENDS server/*.cpp)
file(GLOB_RECURSE H_FILES_SERVER CONFIGURE_DEPENDS server/*.h)

add_executable(${SERVER_NAME} ${C_FILES_SERVER} ${H_FILES_SERVER} ${C_FILES_COMMON} ${H_FILES_COMMON})
target_include_directories(${SERVER_NAME} PUBLIC server dbms common)
add_subdirectory(dbms)
target_link_libraries(${SERVER_NAME} xerces-c dbms)


