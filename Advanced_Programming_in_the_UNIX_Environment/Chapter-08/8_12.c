#include "apue.h"

static void charatattime(char *);

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        charatattime("output from child\n");
    } else {
        charatattime("output form parent\n");
    }

    exit(0);
}

static void charatattime(char *str)
{
    char *ptr;
    int c;

    setbuf(stdout, NULL);

    for (ptr = str; (c = *ptr++) != 0;)
    {
        putc(c, stdout);
    }
}