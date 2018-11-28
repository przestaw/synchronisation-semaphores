#define _XOPEN_SOURCE 7000
// ^ above is important for functions operating on virtual terminals
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include "../include/const.h"
#include "../include/queue.h"
#include "../include/message.h"

FILE *slave1, *slave2, *slave3;

void sigterm_exit(); //TODO

void mem_sem_alloc(); //TODO

int main(void)
{
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = sigterm_exit;
    sigaction(SIGTERM, &action, NULL);
/*
 * First we need to obtain 3 separate slave
 * terminals to see how simulation goes on
 */
    int master;
    char *slavename, window[64], buf[64];
/*
 * SLAVE nr 1
 */
    master = open("/dev/ptmx", O_RDWR);

    grantpt(master);
    unlockpt(master);
    slavename = ptsname(master);

    snprintf(buf, sizeof buf, "-S%s/%d", strrchr(slavename,'/')+1, master);
    if(!fork()) {
        execlp("xterm", "xterm", buf, (char *)0);
        _exit(1);
    }
    slave1 = fopen(slavename, "r+");
    fgets(window, sizeof window, slave1);
    printf("window nr 1: %s\n", window);
/*
 * SLAVE nr 2
 */
    master = open("/dev/ptmx", O_RDWR);

    grantpt(master);
    unlockpt(master);
    slavename = ptsname(master);

    snprintf(buf, sizeof buf, "-S%s/%d", strrchr(slavename,'/')+1, master);
    if(!fork()) {
        execlp("xterm", "xterm", buf, (char *)0);
        _exit(1);
    }
    slave2 = fopen(slavename, "r+");
    fgets(window, sizeof window, slave2);
    printf("window nr 2: %s\n", window);
/*
 * SLAVE nr 3
 */
    master = open("/dev/ptmx", O_RDWR);

    grantpt(master);
    unlockpt(master);
    slavename = ptsname(master);

    snprintf(buf, sizeof buf, "-S%s/%d", strrchr(slavename,'/')+1, master);
    if(!fork()) {
        execlp("xterm", "xterm", buf, (char *)0);
        _exit(1);
    }
    slave3 = fopen(slavename, "r+");
    fgets(window, sizeof window, slave3);
    printf("window nr 3: %s\n", window);
/*
 * Repeated code looks ugly but at least works
 *
 * Each slave will introduce himself with
 */
    fprintf(slave1, "Hello from slave 1\n");
    fprintf(slave2, "Hello from slave 2\n");
    fprintf(slave3, "Hello from slave 3\n");
/*
 * Here is where the program really begins
 */
    printf("Press any key to exit\n");
    getc(stdin);
    return 0;
}

void sigterm_exit()
{
    //TODO: memory and semaphores cleanup
}

void mem_sem_alloc()
{
    //TODO: memory and semaphores creation
}