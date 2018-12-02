//
// Created by przemek on 11/29/18.
//

#include <printf.h>
#include "../include/message.h"

mes_car gen_car(int seed)
{
    mes_car new_car;
    if((seed % 7 == 0) || (seed % 5 == 0) || (seed % 12 == 0))
    //if((seed % 7 == 0) || (seed % 5 == 0) || (seed % 6 == 0) || seed % 3 == 0 || seed % 4 == 0 || seed % 8 == 0)
    {
        new_car.prio = 0;
        if(seed % 31 == 0 || seed % 32 == 0 || seed % 33 == 0)
            new_car.prio = 1;

        new_car.destination = (1 + (seed % 3));

    }else{
        new_car = NOT_A_CAR;
    }
    return new_car;
}
