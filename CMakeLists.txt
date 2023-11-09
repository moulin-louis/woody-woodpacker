cmake_minimum_required(VERSION 3.22)
project(woody_woodpacker C)

set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_C_STANDARD 99)

include_directories(./inc)
add_compile_options(-g -Wall -Werror -Wextra)

add_executable(woody_woodpacker
        src/main.c
        src/utils.c
        src/parsing/segment_header.c
        src/encryption.c
)
