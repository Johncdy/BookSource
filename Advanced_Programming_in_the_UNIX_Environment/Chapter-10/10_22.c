#include "apue.h"

static void sig_int(int);
void pr_mask(const char *);

int main(void)
{
    sigset_t newmask, oldmask, waitmask;

    pr_mask("program start: ");

    if (signal(SIGINT, sig_int) == SIG_ERR)
        err_sys("signal(SIGINT) error");
    sigemptyset(&waitmask);
    sigaddset(&waitmask, SIGUSR1);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGINT);

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");
    
    pr_mask("in critical region: ");

    if (sigsuspend(&waitmask) != -1)
        err_sys("sigsuspend error");
    
    pr_mask("after return from sigsuspend: ");

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");
    
    pr_mask("program exit: ");

    exit(0);
}

static void sig_int(int signo)
{
    pr_mask("\nin sig_int: ");
}

void pr_mask(const char *str)
{
    sigset_t sigset;
    int errno_save;

    errno_save = errno;
    if (sigprocmask(0, NULL, &sigset) < 0) {
        err_ret("sigprocmask error");
    } else {
        printf("%s", str);
        if (sigismember(&sigset, SIGINT))
            printf(" SIGINT");
        if (sigismember(&sigset, SIGQUIT))
            printf(" SIGQUIT");
        if (sigismember(&sigset, SIGUSR1))
            printf(" SIGUSR1");
        if (sigismember(&sigset, SIGALRM))
            printf(" SIGALRM");
        
        printf("\n");
    }

    errno = errno_save;
}