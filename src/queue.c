//
// Created by przemek on 11/27/18.
//

#include <time.h>
#include "../include/queue.h"
#include "../include/message.h"
#include "../include/sem.h"

void init_queue(queue * buf, FILE * stream, int count_sem)
{
    buf->my_stream = stream;
    buf->begin = 0;
    buf->end = 0;
    buf->count_semaphore = count_sem;
}

int put_msg(queue * buf, mes_car car)
{
    if(buf->size == BUF_SIZE -1 || car.prio == -1)
    {
        return -1;
    }else
    {
        buf->size = (buf->size + 1);
        if(car.prio == 1)//if priority move to the bringing
        {
            buf->begin = (buf->begin + 9)% 10; //-1
            buf->car_buf[buf->begin] = car;
        }else
        {
            buf->end = (buf->end + 1)% 10; //+1
            buf->car_buf[buf->end] = car;
        }
        return 0;
    }
}

mes_car see_msg(queue * buf)
{
    mes_car car;
    if(buf->begin == buf->end)
    {
        car = (mes_car){.destination =-1, .prio = -1};
    }else
    {
        car = buf->car_buf[buf->begin];
    }
    return car;
}

void take_car(queue * buf)
{
    if(buf->begin != buf->end)
    {
        buf->begin = (buf->begin + 9)% 10; //-1
        buf->size = (buf->size - 1);
    }
}

int get_size(queue * buf)
{
    return (10 - sem_getVal(buf->count_semaphore));
}

void print_queue(queue * buf)
{
    time_t now;
    struct tm *mytime;
    time(&now);
    mytime = localtime(&now);

    fprintf(buf->my_stream, "%s |-> ", asctime(mytime)); //print when it happend
    for(int i = buf->end; i != buf->begin; i = (i+9)%10 )
    {
        if(buf->car_buf[i].prio == 1)
        {
            fprintf(buf->my_stream, "[P] ");
        }else{
            fprintf(buf->my_stream, "[%d] ", buf->car_buf[i].destination);
        }
    }
    fprintf(buf->my_stream, ":-> \n");
}