cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_COMPILER "g++")

project(lab3-client LANGUAGES CXX)

file(GLOB_RECURSE C_FILES_CLIENT CONFIGURE_DEPENDS liba/*.cpp)
file(GLOB_RECURSE H_FILES_CLIENT CONFIGURE_DEPENDS liba/*.h)

file(GLOB_RECURSE C_FILES_COMMON CONFIGURE_DEPENDS common/*.cxx)
file(GLOB_RECURSE H_FILES_COMMON CONFIGURE_DEPENDS common/*.hxx)

add_executable(${PROJECT_NAME} ${C_FILES_CLIENT} ${H_FILES_CLIENT} ${C_FILES_COMMON} ${H_FILES_COMMON})
target_include_directories(${PROJECT_NAME} PUBLIC liba client common)
add_subdirectory(client)
target_link_libraries(${PROJECT_NAME} xerces-c client)

add_custom_target(testenv
  COMMAND ${CMAKE_COMMAND} -E make_directory testenv)

add_custom_target(test
  DEPENDS testenv
  WORKING_DIRECTORY testenv
  COMMAND ${PROJECT_NAME}
  DEPENDS ${PROJECT_NAME}
)
