#define _XOPEN_SOURCE 7000
// ^ above is important for functions operating on virtual terminals
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <time.h>

#include "../include/const.h"
#include "../include/terminal-stream.h"
#include "../include/sem.h"
#include "../include/message.h"
#include "../include/queue.h"
#include "../include/sh-mem.h"
#include "../include/worker.h"

void sigterm_exit(); //TODO

void mem_sem_alloc(); //TODO

FILE *slave1, *slave2, *slave3;
int queues[3]; //keys to queues
int c_sems[3]; //semids to queue mutex
int m_sems[3]; //semids to queue count semaphores
FILE *slaves[3];

int main(void)
{
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = sigterm_exit;
    sigaction(SIGTERM, &action, NULL);

/*
 * First we need to obtain 3 separate slave
 * terminals to see how simulation goes on
 */
    for(int i = 0; i < 3; i++)
    {
       slaves[i] = get_ptmx_str();
    }
    //slave1 = get_ptmx_str();
    //slave2 = get_ptmx_str();
    //slave3 = get_ptmx_str();
/*
 * Each slave will introduce himself with
 */
    for(int i = 0; i < 3; i++)
    {
       fprintf(slaves[i], "Hello World : from slave %d\n", i+1);
    }
    //fprintf(slave1, "Hello from slave 1\n");
    //fprintf(slave2, "Hello from slave 2\n");
    //fprintf(slave3, "Hello from slave 3\n");
/*
 * Here is where the program really begins
 */
    /*
    for(int i = 0; i < 3; i++)
    {
        printf("printf sem m = %d, sem c =%d\n", m_sems[i], c_sems[i]);
    }

    for(int i = 0; i < 3; i++)
    {
        printf("printf sem m = %d, sem c =%d\n", m_sems[i], c_sems[i]);
    }
    */

    mem_sem_alloc();
    queue *buf[3];
    for(int i = 0; i < 3; i++)
    {
        buf[i] = map_queue(queues[i]);
        init_queue(buf[i], slaves[i], c_sems[i], m_sems[i]);
    }

    if(!fork())
    {
        do_work(queues[2], queues[0], 3);
        _exit(0);
    }

    if(!fork())
    {
        do_work(queues[1], queues[2], 2);
        _exit(0);
    }

    if(!fork())
    {
        do_work(queues[0], queues[1], 1);
        _exit(0);
    }



    if(!fork())
    {
        _exit(1);
    }

/*  temp fin of the program */
    printf("Press any key to exit\n");
    getc(stdin);

    sigterm_exit();
    return 0;
}

void sigterm_exit()
{
    //TODO: memory and semaphores cleanup

    struct shmid_ds arg;
    arg.shm_segsz = sizeof(struct Queue);

    for(int j = 0; j< 3; ++j)
    {
        shmctl(queues[j], IPC_RMID, &arg);
        binary_sem_deallocate(m_sems[j]);
        sem_deallocate(c_sems[j]);
    }
}

void mem_sem_alloc()
{
    //TODO: memory and semaphores creation
    for(int i = 0; i < 3; ++i)
    {
        queues[i] = alloc_sh_queue();
    }
    m_sems[0] = binary_sem_allocate(QUEUE_1, IPC_CREAT | 0666);
    c_sems[0] = sem_allocate(F_QUEUE_1, IPC_CREAT | 0666);
    m_sems[1] = binary_sem_allocate(QUEUE_2, IPC_CREAT | 0666);
    c_sems[1] = sem_allocate(F_QUEUE_2, IPC_CREAT | 0666);
    m_sems[2] = binary_sem_allocate(QUEUE_3, IPC_CREAT | 0666);
    c_sems[2] = sem_allocate(F_QUEUE_3, IPC_CREAT | 0666);
}