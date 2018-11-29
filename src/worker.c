//
// Created by przemek on 11/29/18.
//

#include "../include/worker.h"
#include "../include/sh-mem.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

void do_work(int input, int output, int nr)
{
    queue *in, *out;
    in = map_queue(input);
    out = map_queue(output);

    srand(time(NULL));


    for(int i = 0; i < 200; i++)
    {
        put_msg(out, gen_car(rand()));
        print_queue(out);
    }
}
