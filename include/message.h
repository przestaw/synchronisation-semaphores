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
//gen car from one seed

mes_car gen_new_car(int, int, int);
//gen car from 3 seeds

#endif //MESSAGE_PS