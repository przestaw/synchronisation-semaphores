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
#include "../include/sem.h"
#include "../include/message.h"
#include "../include/queue.h"
#include "../include/sh-mem.h"
#include "../include/worker.h"


FILE *slave1, *slave2, *slave3;

void sigterm_exit(); //TODO

void mem_sem_alloc(); //TODO

int queue1, queue2, queue3; //keys to queues
int m_sem1, m_sem2, m_sem3; //semids to queue mutex

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
    int master;
    char *slavename, window[64], buf[64];
/*
 * SLAVE nr 1
 */
    master = open("/dev/ptmx", O_RDWR);

    grantpt(master);
    unlockpt(master);
    slavename = ptsname(master);

    snprintf(buf, sizeof buf, "-S%s/%d", strrchr(slavename,'/')+1, master);
    if(!fork()) {
        execlp("xterm", "xterm", buf, (char *)0);
        _exit(1);
    }
    slave1 = fopen(slavename, "r+");
    fgets(window, sizeof window, slave1);
    printf("window nr 1: %s\n", window);
/*
 * SLAVE nr 2
 */
    master = open("/dev/ptmx", O_RDWR);

    grantpt(master);
    unlockpt(master);
    slavename = ptsname(master);

    snprintf(buf, sizeof buf, "-S%s/%d", strrchr(slavename,'/')+1, master);
    if(!fork()) {
        execlp("xterm", "xterm", buf, (char *)0);
        _exit(1);
    }
    slave2 = fopen(slavename, "r+");
    fgets(window, sizeof window, slave2);
    printf("window nr 2: %s\n", window);
/*
 * SLAVE nr 3
 */
    master = open("/dev/ptmx", O_RDWR);

    grantpt(master);
    unlockpt(master);
    slavename = ptsname(master);

    snprintf(buf, sizeof buf, "-S%s/%d", strrchr(slavename,'/')+1, master);
    if(!fork()) {
        execlp("xterm", "xterm", buf, (char *)0);
        _exit(1);
    }
    slave3 = fopen(slavename, "r+");
    fgets(window, sizeof window, slave3);
    printf("window nr 3: %s\n", window);
/*
 * Repeated code looks ugly but at least works
 *
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
    init_queue(buf1,slave1,0);
/*  temp test 1 for mem */
    mes_car car = {.destination = 1, .prio = 1};
    put_msg(buf1, gen_car(700));
    put_msg(buf1, gen_car(1428));
    put_msg(buf1, gen_car(714));
    put_msg(buf1, gen_car(1407));
    put_msg(buf1, car);
    if(1)
    {
        print_queue(buf1);
    }
/*  temp test 2 for fork and mem */
    buf2 = map_queue(queue2);
    init_queue(buf2,slave2,0);

    if(!fork())
    {
        do_work(queue1, queue2, 1);
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
}