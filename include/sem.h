//
// Created by przemek on 11/28/18.
//

#ifndef SEM_PS
#define SEM_PS

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <errno.h>

union semun {
    int val;
    struct semid_ds *buf;
    ushort* array;
}; //union for POSIX semapore functions


int binary_sem_allocate(key_t, int);
//create binary semaphore


int binary_sem_deallocate(int);
//delete binary semaphore


int binary_sem_init(int);
//initialize the binary semaphore with the value of 1


int binary_sem_signal(int);
//wait until the value of the semaphore is positive, then decrement


int binary_sem_wait(int);
//increment the semaphore value by 1, after critical section exit


int sem_allocate(key_t, int);
//create a semaphore with multiple values


int sem_deallocate(int);
//deallocate a semaphore


int sem_init(int, int);
//initialize the semaphore with a given value


int sem_check(int);
//checks the value of the semaphore, if positive - decrement
//return 0, if the value of semaphore was 0


int sem_change(int, int);
//change the value of the semaphore by 1 or -1


int sem_getVal(int);
//returns value of the semaphore


#endif //SEM_PS
