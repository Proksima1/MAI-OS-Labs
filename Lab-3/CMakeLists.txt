cmake_minimum_required(VERSION 3.22)
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

add_executable(OS_LABS main.c src/utils.c src/parent.c)

target_include_directories(OS_LABS PRIVATE include)

add_executable(child src/child.c src/utils.c)
target_include_directories(child PRIVATE include)

add_dependencies(OS_LABS child)

#add_executable(reader reader.c)


#add_executable(${CMAKE_PROJECT_NAME}_tests tests/lab1_tests.cpp src/parent.c src/utils.c)
#target_link_libraries(${CMAKE_PROJECT_NAME}_tests PRIVATE GTest::GTest)
#target_include_directories(${CMAKE_PROJECT_NAME}_tests PRIVATE include)
#
#add_test(NAME ${CMAKE_PROJECT_NAME}_tests COMMAND ${CMAKE_PROJECT_NAME}_tests)