#include "apue.h"
#include <fcntl.h>
#include <errno.h>

#define BUFFSIZE    100

typedef void Sigfunc(int);

Sigfunc *signal_intr(int signo, Sigfunc *func)
{
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
#ifdef SA_INTERRUPT
    act.sa_flasg |= SA_INTERRUPT;
#endif
    if (sigaction(signo, &act, &oact) < 0)
        return SIG_ERR;
    return oact.sa_handler;
}

static void sig_xfsz(int signo)
{
    printf("\nin sig_xfsz\n");
}

int main(void)
{
    if (signal_intr(SIGXFSZ, sig_xfsz) == SIG_ERR)
        err_sys("signal_intr(SIGXFSZ) error");

    int fd1, fd2;
    if ((fd1 = open("foo", O_RDWR)) == -1) {
        printf("open foo failded!\n");
        return -1;
    }

    if ((fd2 = open("foonew", O_RDWR | O_CREAT, 0666)) == -1) {
        printf("open foonew failed!\n");
        return -1;
    }

    int n, ret;
    char buf[BUFFSIZE];

    while ((n = read(fd1, buf, BUFFSIZE)) > 0)
    {
        if ((ret = write(fd2, buf, n)) != n)
            printf("write error %d, errno %d", ret, errno);
    }
    
    if (n < 0)
        err_sys("read error");
    
    exit(0);
}