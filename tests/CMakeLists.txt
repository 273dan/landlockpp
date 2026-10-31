set(LANDLOCKPP_TESTS tests)

add_executable(${LANDLOCKPP_TESTS})

target_sources(${LANDLOCKPP_TESTS} PRIVATE
  main.cpp
  unit/misc.cpp
  unit/rulesets.cpp
  post_restrict/post_restrict.cpp
)

target_include_directories(${LANDLOCKPP_TESTS} PRIVATE
  ${CMAKE_SOURCE_DIR}/third_party
  ${CMAKE_SOURCE_DIR}/tests
)

target_link_libraries(${LANDLOCKPP_TESTS} ${LIB_LANDLOCKPP})

target_compile_definitions(${LANDLOCKPP_TESTS} PRIVATE
  DOCTEST_CONFIG_USE_STD_HEADERS
  LANDLOCKPP_TEST_FILES_DIR="${CMAKE_CURRENT_SOURCE_DIR}/files"
  LANDLOCKPP_TEST_FILES_STAGE_DIR="${CMAKE_CURRENT_BINARY_DIR}/stage"
)


