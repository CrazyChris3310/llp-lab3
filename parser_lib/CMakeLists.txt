cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER "g++")

project(client LANGUAGES CXX)

file(GLOB_RECURSE C_FILES_LIB CONFIGURE_DEPENDS ./ast.cpp)
file(GLOB_RECURSE H_FILES_LIB CONFIGURE_DEPENDS ./ast.h)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser parser.y ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/parser.h)
FLEX_TARGET(MyScanner lexer.l  ${CMAKE_CURRENT_SOURCE_DIR}/lexer.cpp DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/lexer.h)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_library(parser_lib
    STATIC
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
    ${C_FILES_LIB}
    ${H_FILES_LIB}
)
target_include_directories(parser_lib PUBLIC .)