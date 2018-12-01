//
// Created by przemek on 11/27/18.
//
#ifndef QUEUE_PS
#define QUEUE_PS

#include <stdio.h>
#include "message.h"

#define BUF_SIZE 10
typedef struct Queue{
    int size;
    int begin;
    int end;
    mes_car car_buf[BUF_SIZE];
    FILE* my_stream;
    int count_semaphore;
    int mutex_semaphore;
}queue;

int put_msg(queue * , mes_car );

mes_car see_msg(queue *);
void take_car(queue *);

mes_car cmp_msg(queue *, mes_car);

int get_size(queue *);

void print_queue(queue *);

void init_queue(queue *, FILE *, int, int);

#endif //QUEUE_PS