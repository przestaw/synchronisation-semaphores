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

    sem_getVal(in->count_semaphore);

}

void do_work(int input, int output, int nr)
{
    queue *in, *out;
    in = map_queue(input);
    out = map_queue(output);

    srand(time(NULL) + nr);

    mes_car current, car, tmp;
    current = NOT_A_CAR;
    car = NOT_A_CAR;
    for(int i = 0; i < 10000; i++)
    {
        if(car.prio == -1)
        {
            do{
                car = gen_car(rand()); //car wanting to go on crossing
            }while(car.destination == nr);
        }
        binary_sem_wait(in->mutex_semaphore);        //SEMAPHORE in WAIT
        tmp = see_msg(in); //first car from road
        if(tmp.prio <= car.prio )
        {
            current = car;
            car = NOT_A_CAR;
            fprintf(in->my_stream, "       "); //print spacing
            print_queue(in);
            switch (current.prio)
            {
                case 1:
                    fprintf(in->my_stream, " <:P:>\n");
                    break;
                case 0:
                    fprintf(in->my_stream, " < %d >\n", current.destination);
                    break;
                default:
                    fprintf(in->my_stream, "\n");
                    break;
            }
            fflush(in->my_stream);
            binary_sem_signal(in->mutex_semaphore);    //SEMAPHORE in SINGAL
        }else
        {
            current = tmp;
            tmp = NOT_A_CAR;
            take_car(in);

            fprintf(in->my_stream, "       "); //print spacing
            print_queue(in);
            switch (current.prio)
            {
                case 1:
                    fprintf(in->my_stream, " -> [P]");
                    break;
                case 0:
                    fprintf(in->my_stream, " -> [%d]", current.destination);
                    break;
                default:
                    break;
            }
            if(current.destination == nr)
            {
                fprintf(in->my_stream, " *GONE* \n");
            }else
            {
                fprintf(in->my_stream, "\n");
            }
            fflush(in->my_stream);
            binary_sem_signal(in->mutex_semaphore);    //SEMAPHORE in SINGAL
            sem_change(in->count_semaphore, +1);//SEMAPHORE in count UP

        }


        do_sleep(); //rand sleep from 0.85 to 1.15 sec
        //sleep(1);

        if(current.destination == nr)
        {
            current = NOT_A_CAR; //consume car
        }else
        {
            if(current.prio != -1)
            {

                sem_change(out->count_semaphore, -1);
                binary_sem_wait(out->mutex_semaphore);//SEMAPHORE out WAIT
                switch (current.prio)
                {
                    case 1:
                        fprintf(out->my_stream, "[P] -> ");
                        break;
                    case 0:
                        fprintf(out->my_stream, "[%d] -> ", current.destination);
                        break;
                    default:
                        break;
                }
                print_queue(out);
                fprintf(out->my_stream, "\n");
                fflush(out->my_stream);
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
    Xtime.tv_nsec = 185000000 + rand()%300000000;
    nanosleep(&Xtime, NULL);
}