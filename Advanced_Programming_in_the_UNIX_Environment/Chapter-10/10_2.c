#include "apue.h"

static void sig_usr(int);

int main(void)
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
        err_sys("can't catch ");
    }
}