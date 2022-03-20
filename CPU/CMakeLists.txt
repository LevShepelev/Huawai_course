cmake_minimum_required(VERSION 3.1)

project(CPU)

add_executable(
    CPU
    source/CPU.cpp
)

add_executable(
    Disassemblr
    source/Disassemblr.cpp
)

add_executable(
    Assembler
    source/Assembler.cpp
)
include_directories(include)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra  -g")


