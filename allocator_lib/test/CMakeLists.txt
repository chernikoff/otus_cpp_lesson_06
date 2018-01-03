add_executable(tests
  tests.cpp)

set_target_properties(tests
  PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS NO
  )

target_link_libraries(tests
  PRIVATE
  ${GTEST_LIBRARIES}
  allocator_lib
  pthread)

add_test(tests tests)
