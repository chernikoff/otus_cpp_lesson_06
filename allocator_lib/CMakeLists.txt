set(NAME allocator)

add_library(${NAME}_lib
  INTERFACE
  )

target_include_directories(${NAME}_lib
  INTERFACE
  include)

if(GTEST_FOUND)
  add_subdirectory(test)
endif()
