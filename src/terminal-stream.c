//
// Created by przemek on 12/1/18.
//
#define _XOPEN_SOURCE 6756

#include <stdlib.h>
#include <stdio.h>
#include <string.h> //strrchr
#include <unistd.h>
#include <fcntl.h>
#include "../include/terminal-stream.h"


FILE * get_ptmx_str()
{
    FILE * slave;

    char *slavename, window[64], buf[64];

    int master = open("/dev/ptmx", O_RDWR);

    grantpt(master);
    unlockpt(master);
    slavename = ptsname(master);

    snprintf(buf, sizeof buf, "-S%s/%d", strrchr(slavename,'/')+1, master);
    if(!fork()) {
        execlp("xterm", "xterm", buf, (char *)0);
        _exit(1);
    }
    slave = fopen(slavename, "r+");
    fgets(window, sizeof window, slave);
    printf("window nr 1: %s\n", window);

    return slave;
}