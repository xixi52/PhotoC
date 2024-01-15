cmake_minimum_required(VERSION 3.26)
project(PhotoC C)

set(CMAKE_C_STANDARD 11)

add_executable(PhotoC main.c image_operations.h image_operations.c)
