/*************************************************************************
> File: thread_exit.c
> Author: eric.xu
> Mail:   eric.xu@libratone.com.cn
> Time:   2017-12-08 10:15:18
*************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <signal.h>

static int do_abort = 0;

void handle_signal(int signo)
{
    if (signo == SIGHUP) {
        printf("child recv SIGHUP..\n");
        do_abort = 1;
    }
}

int main(void)
{
    pid_t pid;
    pid = fork();
    char* p = NULL;

    if (pid == 0) { // child
        //signal(SIGHUP, handle_signal);
        //prctl(PR_SET_PDEATHSIG, SIGHUP);

        while (!do_abort) {
            sleep(1);
        }

        printf("child exit...\n");
        return 0;
    } else { // parent
        int times = 5;

        while (times -- > 0) {
            sleep(10);
            printf("in parent.\n");
        }

        printf("parent exit..\n");
        return 0;
    }

    return 0;
}
