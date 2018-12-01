//
// Created by przemek on 11/29/18.
//

#include "../include/message.h"

mes_car gen_car(int seed)
{
    mes_car new_car;
    if(seed % 7 == 0)
    {
        new_car.prio = 0;
        if((seed % 13 == 0)||(seed % 15 == 0) || (seed % 18 == 0))
            new_car.prio = 1;

        new_car.destination = (1 + (seed % 3));

    }else{
        new_car = NOT_A_CAR;
    }
    return new_car;
}
