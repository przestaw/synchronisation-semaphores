cmake_minimum_required(VERSION 3.10)
project(SOI_3 C)

set(CMAKE_C_STANDARD 11)

add_executable(SOI_3 src/main.c src/queue.c include/queue.h include/message.h include/const.h src/sem.c include/sem.h src/worker.c include/worker.h src/message.c src/sh-mem.c include/sh-mem.h src/terminal-stream.c include/terminal-stream.h)