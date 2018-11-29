//
// Created by przemek on 11/29/18.
//

#include "../include/sh-mem.h"
#include "../include/queue.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>



int alloc_sh_queue()
{
    int ret;
    ret = shmget(0, sizeof(struct Queue), IPC_CREAT | 0666);
    if(ret < 0)
    {
        printf("shmget error aborting program...");
        _exit(1);
    }
    return ret;
}

void * map_queue(int id)
{
    queue *buf = shmat(id, NULL, 0);
    if((long int)buf == -1)
    {
        printf("shmat error aborting...");
        _exit(1);
    }
}