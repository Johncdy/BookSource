#include "apue.h"

static void charatattime(char *);

int main(void)
{
    pid_t pid;

    TELL_WAIT();

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        WAIT_PARENT();
        charatattime("output from child\n");
        TELL_PARENT(getppid());
    } else {
        charatattime("output form parent\n");
        TELL_CHILD(pid);
        WAIT_CHILD();
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