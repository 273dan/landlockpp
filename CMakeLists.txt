cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(landlockpp)

set(LIB_LANDLOCKPP landlockpp)

add_library(${LIB_LANDLOCKPP} INTERFACE)

target_include_directories(${LIB_LANDLOCKPP} INTERFACE include)

option(LANDLOCKPP_ENABLE_TESTS "Build landlockpp tests" ON)

if(${LANDLOCKPP_ENABLE_TESTS})
  add_subdirectory(tests)
endif()
