cmake_minimum_required(VERSION 3.1)
project (Asyncpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXE_LINKER_FLAGS "-lrt -lpthread -ldl -std=c++11")

add_compile_options(-lrt -lpthread -ldl -std=c++11)

include_directories(./)

file(GLOB SOURCES "core/src/*.cpp" "util/src/*.cpp" "*.cpp" "ipc/src/*.cpp" "async/src/*.cpp" "timer/src/*.cpp")

add_executable(Asyncpp ${SOURCES})
target_link_libraries(Asyncpp ${CMAKE_EXE_LINKER_FLAGS})

