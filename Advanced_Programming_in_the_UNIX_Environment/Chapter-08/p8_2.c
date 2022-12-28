#include "apue.h"

static void f1(void), f2(void);

int main(void)
{
    f1();
    f2();
    exit(0);
}

static void f1(void) {
    pid_t pid;

    if ((pid = vfork()) < 0)
        err_sys("vfork error");
}

static void f2(void) {
    char buf[1000];
    int i;

    for (i = 0; i < sizeof(buf); i++)
    {
        buf[i] = 0;
    }
}