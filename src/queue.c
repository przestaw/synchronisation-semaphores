//
// Created by przemek on 11/27/18.
//

#include "../include/queue.h"
#include "../include/message.h"

int put_msg(buffer * buf, mes_car car)
{
    if(buf->size == BUF_SIZE)
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

mes_car get_msg(buffer * buf)
{
    mes_car car;

    if(buf->begin != buf->end)
    {
        car = buf->car_buf[buf->begin];
        buf->begin = (buf->begin + 1)% 10; //+1
        buf->size = (buf->size - 1);
    }else{
        car.prio = -1;
        car.destination = -1;
    }
    return car;
}
