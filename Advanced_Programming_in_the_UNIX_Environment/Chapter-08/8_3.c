#include "apue.h"

int globvar = 6;

int main(void)
{
    int var;
    pid_t pid;

    var = 88;
    printf("before fork\n");
    if ((pid = vfork()) < 0) {
        err_sys("vfork error");
    } else if (pid == 0) {
        globvar++;
        var++;
        fclose(stdout);
        exit(0);
    }

    int t = printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);
    printf("%d\n", t);

    exit(0);
}