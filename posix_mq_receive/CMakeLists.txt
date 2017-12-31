cmake_minimum_required(VERSION 3.9)
project(posix_mq_receive)

set(CMAKE_CXX_STANDARD 98)
set(GCC_COVERATE_LINK_FLAGS "-lrt")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERATE_LINK_FLAGS}")

add_executable(posix_mq_receive main.cpp)