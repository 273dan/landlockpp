cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(landlockpp)

set(LANDLOCKPP_LIB landlockpp)
set(LANDLOCKPP_WHICH_VER which_version)

add_library(${LANDLOCKPP_LIB} INTERFACE)

target_include_directories(${LANDLOCKPP_LIB} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>


)

target_compile_features(${LANDLOCKPP_LIB} INTERFACE cxx_std_23)

option(LANDLOCKPP_ENABLE_TESTS "Build landlockpp tests" ${PROJECT_IS_TOP_LEVEL})
option(LANDLOCKPP_ENABLE_WHICH_VER "Build which version utility" ${PROJECT_IS_TOP_LEVEL}) 

if(${LANDLOCKPP_ENABLE_TESTS})
  add_subdirectory(tests)
endif()

if(${LANDLOCKPP_ENABLE_WHICH_VER})
  add_executable(${LANDLOCKPP_WHICH_VER} which_version/which_version.cpp)
  target_link_libraries(${LANDLOCKPP_WHICH_VER} ${LANDLOCKPP_LIB})
endif()
