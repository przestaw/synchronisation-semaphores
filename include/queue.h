//
// Created by przemek on 11/27/18.
//
#ifndef QUEUE_PS
#define QUEUE_PS

#include <stdio.h>
#include "message.h"

#define BUF_SIZE 10

typedef struct{
    int size;
    int begin;
    int end;
    mes_car car_buf[BUF_SIZE];
    FILE* my_stream;
} buffer;

#endif //QUEUE_PS