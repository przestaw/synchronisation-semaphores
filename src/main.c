#define _XOPEN_SOURCE 7000

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "../include/const.h"
#include "../include/queue.h"
#include "../include/message.h"

int main(void)
{

    int master;
    char *slavename, window[64], buf[64];
    FILE *slave;

    //master = posix_openpt(O_RDWR);

    master = open("/dev/ptmx", O_RDWR);

    grantpt(master);
    unlockpt(master);
    slavename = ptsname(master);

    printf("master: %d\n", master);
    printf("slavename: %s\n", slavename);

    snprintf(buf, sizeof buf, "-S%s/%d", strrchr(slavename,'/')+1, master);
    if(!fork()) {
        execlp("xterm", "xterm", buf, (char *)0);
        _exit(1);
    }
    slave = fopen(slavename, "r+");
    fgets(window, sizeof window, slave);
    printf("window: %s\n", window);


    fputs("say something: ", slave);
    fgets(buf, sizeof buf, slave);
    fprintf(slave, "you said %s\nexiting in 3 seconds...\n", buf);
    sleep(3);
    return 0;
}
