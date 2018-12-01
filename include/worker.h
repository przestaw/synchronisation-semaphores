//
// Created by przemek on 11/29/18.
//

#ifndef WORKER_PS
#define WORKER_PS

#include "../include/queue.h"

void do_work(int input, int output, int nr);

void do_work_old(int input, int output, int nr);

void * map_queue(int);

#endif //WORKER_PS
