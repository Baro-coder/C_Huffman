cmake_minimum_required(VERSION 3.10)

project(huffman)

include_directories(include)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections")

file(GLOB_RECURSE SOURCES src/*.c)
set(ALL_SOURCES ${SOURCES})

add_executable(${CMAKE_PROJECT_NAME} ${ALL_SOURCES})
