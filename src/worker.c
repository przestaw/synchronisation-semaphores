//
// Created by przemek on 11/29/18.
//

#include "../include/worker.h"
#include "../include/sh-mem.h"
#include "../include/sem.h"

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

void do_sleep();

void do_work_old(int input, int output, int nr)
{
    queue *in, *out;
    in = map_queue(input);
    out = map_queue(output);

    srand(time(NULL) + nr);


    for(int i = 0; i < 300; i++)
    {
        put_msg(out, gen_car(rand()));
        print_queue(out);
        fprintf(out->my_stream, "\n"); //flush stream
    }
}

void do_work(int input, int output, int nr)
{
    queue *in, *out;
    in = map_queue(input);
    out = map_queue(output);

    srand(time(NULL) + nr);

    mes_car current, car, tmp;
    current = NOT_A_CAR;

    while(1)
    {
        car = gen_car(rand()); //car wanting to go on crossing
        binary_sem_wait(in->mutex_semaphore);        //SEMAPHORE in WAIT
        tmp = see_msg(in); //car from road
        if(tmp.prio <= car.prio )
        {
            current = car;
            car = NOT_A_CAR;
            print_queue(in);
            if (current.prio == 1)
            {
                fprintf(in->my_stream, " >:P:<\n");
            }else
            if(current.prio == 0)
            {
                fprintf(in->my_stream, " >:%d:<\n", current.destination);
            }else
                {
                    fprintf(in->my_stream, "\n");
                }
        }else
        {
            current = tmp;
            tmp = NOT_A_CAR;
            take_car(in);
            //sem_change(in->count_semaphore, 1);//SEMAPHORE in count UP
            print_queue(in);
            if (current.prio == 1)
            {
                fprintf(in->my_stream, " -> [P]\n");
            }else
            {
                fprintf(in->my_stream, " -> [%d]\n", current.destination);
            }
        }
        binary_sem_signal(in->mutex_semaphore);    //SEMAPHORE in SINGAL
        fflush(in->my_stream);

        do_sleep(); //rand sleep from 0.85 to 1.15 sec
        //sleep(1);
        if(current.destination == nr)
        {
            current = NOT_A_CAR; //consume car
        }else
        {
            if(current.prio != -1)
            {
                //sem_change(out->count_semaphore, -1);//SEMAPHORE out count DOWN-wait
                binary_sem_wait(out->mutex_semaphore);//SEMAPHORE out WAIT
                if (current.prio == 1)
                {
                    fprintf(out->my_stream, "[P] -> ");
                }else
                {
                    fprintf(out->my_stream, "[%d] -> ", current.destination);
                }
                print_queue(out);
                fprintf(out->my_stream, "\n"); //flush stream
                put_msg(out, current); //here to don't interrupt printing
                binary_sem_signal(out->mutex_semaphore);//SEMAPHORE out SIGNAL
                current = NOT_A_CAR;
            }
        }
    }
}

void do_sleep()
{
    struct timespec Xtime;
    Xtime.tv_sec = 0;
    Xtime.tv_nsec = 85000000 + rand()%300000000;
    nanosleep(&Xtime, NULL);
}