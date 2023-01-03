#include "apue.h"

static void pr_ids(char *name)
{
    printf("%s: pid = %ld, ppgid = %ld, pgrp = %ld, tpgrp = %ld\n", name, (long)getpid(), (long)getppid(), (long)getpgrp(), (long)tcgetpgrp(STDIN_FILENO));
    fflush(stdout);
}

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {
        sleep(5);
    } else {
        setsid();
        pr_ids("child");
    }
    exit(0);
}