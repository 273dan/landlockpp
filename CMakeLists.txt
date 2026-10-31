cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(landlockpp)

set(LIB_LANDLOCKPP landlockpp)
set(LANDLOCKPP_WHICH_VER which_version)

add_library(${LIB_LANDLOCKPP} INTERFACE)

target_include_directories(${LIB_LANDLOCKPP} INTERFACE include)

option(LANDLOCKPP_ENABLE_TESTS "Build landlockpp tests" ON)
option(LANDLOCKPP_ENABLE_WHICH_VER "Build which version utility" ON) 

if(${LANDLOCKPP_ENABLE_TESTS})
  add_subdirectory(tests)
endif()

if(${LANDLOCKPP_ENABLE_WHICH_VER})
  add_executable(${LANDLOCKPP_WHICH_VER} which_version/which_version.cpp)
  target_link_libraries(${LANDLOCKPP_WHICH_VER} ${LIB_LANDLOCKPP})
endif()
