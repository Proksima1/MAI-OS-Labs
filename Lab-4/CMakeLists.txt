cmake_minimum_required(VERSION 3.20)
project(OS_LABS C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

add_library(lib_funcs SHARED lib/src/pi.c lib/src/gcd.c)
target_include_directories(lib_funcs PUBLIC lib/include)

add_executable(program1 program_static.c)
target_link_libraries(program1 lib_funcs)

add_executable(program2 program_dynamic.c)
target_link_libraries(program2 dl)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)