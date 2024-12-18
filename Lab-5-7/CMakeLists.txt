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

FetchContent_Declare(
        cppzmq
        GIT_REPOSITORY https://github.com/zeromq/cppzmq.git
        GIT_TAG v4.10.0
        TLS_VERIFY false
)

FetchContent_MakeAvailable(cppzmq)


#find_package(cppzmq REQUIRED)

add_executable(OS_LABS main.cpp src/controller.cpp src/utils.cpp)
target_include_directories(OS_LABS PRIVATE include)

target_include_directories(OS_LABS PRIVATE ${cppzmq_SOURCE_DIR})
target_link_libraries(OS_LABS PRIVATE zmq)


add_executable(worker src/worker.cpp src/utils.cpp src/worker_node.cpp)
target_include_directories(worker PRIVATE include)

target_include_directories(worker PRIVATE ${cppzmq_SOURCE_DIR})
target_link_libraries(worker PRIVATE zmq)


add_dependencies(OS_LABS worker)

enable_testing()
#
add_executable(test_worker tests/test_worker.cpp src/worker_node.cpp src/utils.cpp)
target_link_libraries(test_worker PRIVATE GTest::GTest zmq)
target_include_directories(test_worker PRIVATE include ${cppzmq_SOURCE_DIR})

add_test(NAME ${CMAKE_PROJECT_NAME}_tests COMMAND ${CMAKE_PROJECT_NAME}_tests)