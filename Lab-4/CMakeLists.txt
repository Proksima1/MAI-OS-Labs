cmake_minimum_required(VERSION 3.20)
project(OS_LABS C CXX)

set(CMAKE_C_STANDARD 11)
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

add_library(lib_funcs SHARED lib/src/pi.c lib/src/gcd.c)
target_include_directories(lib_funcs PUBLIC lib/include)

add_executable(program1 program_static.c)
target_link_libraries(program1 lib_funcs)

add_executable(program2 program_dynamic.c)
target_link_libraries(program2 dl)

enable_testing()

add_executable(${CMAKE_PROJECT_NAME}_tests tests/lab4_tests.cpp)
target_include_directories(${CMAKE_PROJECT_NAME}_tests PRIVATE include)
target_link_libraries(${CMAKE_PROJECT_NAME}_tests PRIVATE GTest::GTest lib_funcs)


add_test(NAME ${CMAKE_PROJECT_NAME}_tests COMMAND ${CMAKE_PROJECT_NAME}_tests)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

