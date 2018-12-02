//
// Created by przemek on 11/27/18.
//

#include <time.h>
#include "../include/queue.h"
#include "../include/message.h"
#include "../include/sem.h"

void init_queue(queue * buf, FILE * stream, int count_sem, int mut_sem)
{
    buf->my_stream = stream;
    buf->begin = 0;
    buf->end = 0;
    buf->count_semaphore = count_sem;
    sem_init(count_sem, BUF_SIZE);
    buf->mutex_semaphore = mut_sem;
    binary_sem_init(mut_sem);
    for(int i = 0; i < BUF_SIZE; i++)
    {
        buf->car_buf[i] = NOT_A_CAR;
    }
}

int put_msg(queue * buf, mes_car car)
{
    if(buf->size >= BUF_SIZE)
    {
        printf("problem - sem = %d", sem_getVal(buf->count_semaphore));
        return -1;
    }
    if(car.prio == -1)
    {
        return -1;
    }else
    {
        buf->size = buf->size + 1;
        if(car.prio == 1)//if priority move to the begining
        {
            buf->begin = (buf->begin + 9)% BUF_SIZE; //-1
            buf->car_buf[buf->begin] = car;
        }else
        {
            buf->car_buf[buf->end] = car;
            buf->end = (buf->end + 1)% BUF_SIZE; //+1
        }
        return 0;
    }
}

mes_car see_msg(queue * buf)
{
    mes_car car;
    if(buf->size == 0)
    {
        car = NOT_A_CAR;
    }else
    {
        car = buf->car_buf[buf->begin];
    }
    return car;
}

void take_car(queue * buf)
{
    if(buf->size != 10)
    {
        buf->car_buf[buf->begin] = NOT_A_CAR;
        buf->begin = (buf->begin + 1)% BUF_SIZE; //+1
        buf->size = (buf->size - 1);
    }else
    {
        printf("XD"); //DEBUG
    }
}

mes_car cmp_msg(queue * buf, mes_car car)
{
    mes_car tmp = see_msg(buf);
    if(tmp.prio <= car.prio )
    {
        print_queue(buf);
        fprintf(buf->my_stream, "\n");
        return car;
    }else
    {
        take_car(buf);
        if (tmp.prio == 1)
        {
            fprintf(buf->my_stream, "[P]\n");
        }else
        {
            fprintf(buf->my_stream, "[%d]\n", tmp.destination);
        }
        return tmp;
    }
}

int get_size(queue * buf)
{
    return (10 - sem_getVal(buf->count_semaphore));
}

void print_queue(queue * buf)
{
/*
    time_t now;
    struct tm *mytime;
    time(&now);
    mytime = localtime(&now);
*/
// asctime(mytime) -> for time??
    int i = (buf->begin + 9 )% BUF_SIZE;
    fprintf(buf->my_stream, "| "); //print when it happend
    for(int j = 0; j < 10; j++)
    {
        switch (buf->car_buf[i].prio)
        {
            case 1:
                fprintf(buf->my_stream, "[P] ");
                break;
            case 0:
                fprintf(buf->my_stream, "[%d] ", buf->car_buf[i].destination);
                break;
            case -1:
                fprintf(buf->my_stream, "[ ] ");
                break;
            default:
                break;
        }
        i = (i+9)%10;
    }
    fprintf(buf->my_stream, ": ");
}