cmake_minimum_required(VERSION 3.22)
project(OS_LABS CXX)

set(CMAKE_CXX_STANDARD 20)


include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
        TLS_VERIFY false
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(OS_LABS main.cpp src/tools.cpp)
target_include_directories(OS_LABS PRIVATE include)

add_executable(${CMAKE_PROJECT_NAME}_tests tests/tests.cpp src/tools.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME}_tests PRIVATE GTest::GTest)
target_include_directories(${CMAKE_PROJECT_NAME}_tests PRIVATE include)

add_test(NAME ${CMAKE_PROJECT_NAME}_tests COMMAND ${CMAKE_PROJECT_NAME}_tests)