set(LANDLOCKPP_TESTS tests)

add_executable(${LANDLOCKPP_TESTS})

target_sources(${LANDLOCKPP_TESTS} PRIVATE
  main.cpp
  tests.cpp
)

target_include_directories(${LANDLOCKPP_TESTS} PRIVATE
  ${CMAKE_SOURCE_DIR}/third_party
)

target_link_libraries(${LANDLOCKPP_TESTS} ${LIB_LANDLOCKPP})

target_compile_definitions(${LANDLOCKPP_TESTS} PRIVATE DOCTEST_CONFIG_USE_STD_HEADERS)
