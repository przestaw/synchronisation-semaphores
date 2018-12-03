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

void sigterm_exit();

void mem_sem_alloc();

void kill_workers();

void fork_workers();

int queues[3]; //keys to queues
int c_sems[3]; //semids to queue mutex
int m_sems[3]; //semids to queue count semaphores
FILE *slaves[3]; //terminal streams
pid_t worker_PID[3]; //worker PID-s
queue *buf[3]; //maped queues to initialize

int main(void)
{
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = sigterm_exit;
    sigaction(SIGTERM, &action, NULL);
    mem_sem_alloc();

/*
 * We need to obtain 3 separate slave
 * terminals to see how simulation goes on
 */
    for(int i = 0; i < 3; i++)
    {
       slaves[i] = get_ptmx_str();
    }
/*
 * Each slave will introduce himself with
 */
    for(int i = 0; i < 3; i++)
    {
       fprintf(slaves[i], "Hello World : from slave %d\n", i+1);
    }
/*
 * Here is where the program begins
 * now we initialize queues
 * then menu
 */

    for(int i = 0; i < 3; i++)
    {
        buf[i] = map_queue(queues[i]);
        init_queue(buf[i], slaves[i], c_sems[i], m_sems[i]);
    }

    int exit = 0;
    int input1, input2;
    while(!exit)
    {
        printf("Choose option:\n"
               "\t1. Begin\n"
               "\t2. Check queue count\n"
               "\t3. Exit\n");
        if(scanf("%d", &input1) != 1)
        {
            input1 = 0;
        }
        while(getc(stdin) != '\n');
        switch (input1)
        {
            case 1 :
                fork_workers();
                break;
            case 2 :
            {
                printf("\tChoose Queue:\n"
                       "\t\t1. Queue\n"
                       "\t\t2. Queue\n"
                       "\t\t3. Queue\n");
                if(scanf("%d", &input2) != 1)
                {
                    input2 = 0;
                }
                while(getc(stdin) != '\n');
                switch (input2)
                {
                    case 1 :
                    case 2 :
                    case 3 :
                        printf("Queue %d is %d cars\n", input2, get_size(buf[input2-1]));
                        break;
                    default:
                        printf("Not an option\n");
                        break;
                }
            }
                break;
            case 3 :
                exit = 1;
                break;
            default:
                printf("Not an option\n");
                break;
        }
    }
/*  temp fin of the program */
//    printf("Press any key to exit\n");
//    getc(stdin);
    sigterm_exit();
    return 0;
}

void sigterm_exit() //deallock shared memory o exit/sigterm
{
    printf("Cleaning...\n");

    kill_workers();

    struct shmid_ds arg;
    arg.shm_segsz = sizeof(struct Queue);

    for(int j = 0; j< 3; ++j)
    {
        shmctl(queues[j], IPC_RMID, &arg);
        binary_sem_deallocate(m_sems[j]);
        sem_deallocate(c_sems[j]);
    }

    printf("Cleaned.\n");
}

void mem_sem_alloc() //alloc shared memory
{
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

void kill_workers() //send sigkill to workers as they are simplified to infinite loop
{
    for(int i = 0; i < 3; i++)
    {
        if(worker_PID[i])
        {
            kill(worker_PID[i], SIGKILL);
            worker_PID[i] = 0;
        }else{
            printf("No process to kill\n");
            return;
        }
    }
    for(int i = 0; i < 3; i++)
    {
        init_queue(buf[i], slaves[i], c_sems[i], m_sems[i]);
        fprintf(slaves[i],"Cleaning queue nr %d\n", i+1);
    }
}

void fork_workers() //frok junction worker process
{
    if(worker_PID[0] != 0)
    {
        printf("Already forked workers\n");
        return;
    }
    for(int i = 0; i < 3; i++)
    {
        if(!(worker_PID[i]=fork()))
        {
            do_work(queues[i], queues[(i+1)%3], i+1);
            _exit(0);
        }
    }
}