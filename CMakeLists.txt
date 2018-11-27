cmake_minimum_required(VERSION 3.10)
project(SOI_3 C)

set(CMAKE_C_STANDARD 11)

add_executable(SOI_3 src/main.c src/queue.c include/queue.h include/message.h include/const.h)