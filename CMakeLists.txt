cmake_minimum_required(VERSION 3.30)

project(huffman)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAG "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pedantic")

add_subdirectory(app)
add_subdirectory(lib)