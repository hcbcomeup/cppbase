cmake_minimum_required(VERSION 3.15)
project(cppbase)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB_RECURSE CPPBASE_SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_library(cppbase STATIC ${CPPBASE_SRC_FILES})


option(BUILD_TEST "Build test cases for cppbase" OFF)
if(BUILD_TEST)
    add_subdirectory(test)
endif()

option(BUILD_EXAMPLES "Build examples for cppbase" OFF)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()