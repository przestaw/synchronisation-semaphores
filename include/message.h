//
// Created by przemek on 11/27/18.
//

#ifndef MESSAGE_PS
#define MESSAGE_PS

#define NOT_A_CAR (mes_car){.prio = -1, .destination = -1}

typedef struct Mes_Car{
    int destination;
    int prio;
    //place for value
}mes_car;

mes_car gen_car(int);

#endif //MESSAGE_PS