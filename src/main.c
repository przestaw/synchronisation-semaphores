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

int queue1, queue2, queue3; //keys to queues
int m_sem1, m_sem2, m_sem3; //semids to queue mutex
int c_sem1, c_sem2, c_sem3; //semids to queue count semaphores
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
    slave1 = get_ptmx_str();
    slave2 = get_ptmx_str();
    slave3 = get_ptmx_str();
/*
 * Each slave will introduce himself with
 */
    fprintf(slave1, "Hello from slave 1\n");
    fprintf(slave2, "Hello from slave 2\n");
    fprintf(slave3, "Hello from slave 3\n");
/*
 * Here is where the program really begins
 */
    struct Queue *buf1, *buf2, *buf3;
    mem_sem_alloc();

    buf1 = map_queue(queue1);
    init_queue(buf1, slave1, c_sem1, m_sem1);
    buf2 = map_queue(queue2);
    init_queue(buf2, slave2, c_sem2, m_sem2);
    buf3 = map_queue(queue3);
    init_queue(buf3, slave3, c_sem3, m_sem3);


    if(!fork())
    {
        do_work(queue1, queue2, 1);
        _exit(0);
    }

    if(!fork())
    {
        do_work(queue2, queue3, 2);
        _exit(0);
    }

    if(!fork())
    {
        do_work(queue3, queue1, 3);
        _exit(0);
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

    shmctl(queue1, IPC_RMID, &arg);
    shmctl(queue2, IPC_RMID, &arg);
    shmctl(queue3, IPC_RMID, &arg);

    binary_sem_deallocate(m_sem1);
    binary_sem_deallocate(m_sem2);
    binary_sem_deallocate(m_sem3);

    sem_deallocate(c_sem1);
    sem_deallocate(c_sem2);
    sem_deallocate(c_sem3);
}

void mem_sem_alloc()
{
    //TODO: memory and semaphores creation
    queue1 = alloc_sh_queue();
    queue2 = alloc_sh_queue();
    queue3 = alloc_sh_queue();

    m_sem1 = binary_sem_allocate(QUEUE_1, IPC_CREAT | 0666);
    m_sem2 = binary_sem_allocate(QUEUE_2, IPC_CREAT | 0666);
    m_sem3 = binary_sem_allocate(QUEUE_3, IPC_CREAT | 0666);

    binary_sem_init(m_sem1);
    binary_sem_init(m_sem2);
    binary_sem_init(m_sem3);

    c_sem1 = sem_allocate(F_QUEUE_1, IPC_CREAT | 0666);
    c_sem2 = sem_allocate(F_QUEUE_2, IPC_CREAT | 0666);
    c_sem3 = sem_allocate(F_QUEUE_3, IPC_CREAT | 0666);

    sem_init(c_sem1, 10);
    sem_init(c_sem2, 10);
    sem_init(c_sem3, 10);
}