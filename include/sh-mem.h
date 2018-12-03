//
// Created by przemek on 11/29/18.
//

#ifndef SH_MEM_PS
#define SH_MEM_PS

int alloc_sh_queue();
//alloc shared memory struct

void * map_queue(int);
//map queue to adress space of process

#endif //SH_MEM_PS
