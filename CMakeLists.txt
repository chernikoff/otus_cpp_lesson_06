cmake_minimum_required(VERSION 3.2)

project(otus_cpp_lesson_06 VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(GTest)

enable_testing()

add_subdirectory(allocator_lib)
add_subdirectory(allocator)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT chernikoff.alex@gmail.com)

include(CPack)
