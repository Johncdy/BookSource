#include "apue.h"
#include <errno.h>

void pr_mask(const char *str)
{
    sigset_t sigset;
    int errno_save;

    errno_save = errno;
    if (sigprocmask(0, NULL, &sigset) < 0) {
        err_ret("sigprocmask error");
    } else {
        printf("%s", str);
        if (sigismember(&sigset, SIGHUP))
            printf(" SIGHUP");
        if (sigismember(&sigset, SIGINT))
            printf(" SIGINT");
        if (sigismember(&sigset, SIGQUIT))
            printf(" SIGQUIT");
        if (sigismember(&sigset, SIGILL))
            printf(" SIGILL");
        if (sigismember(&sigset, SIGTRAP))
            printf(" SIGTRAP");
        if (sigismember(&sigset, SIGABRT))
            printf(" SIGABRT");
        if (sigismember(&sigset, SIGBUS))
            printf(" SIGBUS");
        if (sigismember(&sigset, SIGFPE))
            printf(" SIGFPE");
        if (sigismember(&sigset, SIGKILL))
            printf(" SIGKILL");
        if (sigismember(&sigset, SIGUSR1))
            printf(" SIGUSR1");
        if (sigismember(&sigset, SIGSEGV))
            printf(" SIGSEGV");
        if (sigismember(&sigset, SIGUSR2))
            printf(" SIGUSR2");
        if (sigismember(&sigset, SIGPIPE))
            printf(" SIGPIPE");
        if (sigismember(&sigset, SIGALRM))
            printf(" SIGALRM");
        if (sigismember(&sigset, SIGTERM))
            printf(" SIGTERM");
        if (sigismember(&sigset, SIGSTKFLT))
            printf(" SIGSTKFLT");
        if (sigismember(&sigset, SIGCHLD))
            printf(" SIGCHLD");
        if (sigismember(&sigset, SIGCONT))
            printf(" SIGCONT");
        if (sigismember(&sigset, SIGSTOP))
            printf(" SIGSTOP");
        if (sigismember(&sigset, SIGTSTP))
            printf(" SIGTSTP");
        if (sigismember(&sigset, SIGTTIN))
            printf(" SIGTTIN");
        if (sigismember(&sigset, SIGTTOU))
            printf(" SIGTTOU");
        if (sigismember(&sigset, SIGURG))
            printf(" SIGURG");
        if (sigismember(&sigset, SIGXCPU))
            printf(" SIGXCPU");
        if (sigismember(&sigset, SIGXFSZ))
            printf(" SIGXFSZ");
        if (sigismember(&sigset, SIGVTALRM))
            printf(" SIGVTALRM");
        if (sigismember(&sigset, SIGPROF))
            printf(" SIGPROF");
        if (sigismember(&sigset, SIGWINCH))
            printf(" SIGWINCH");
        if (sigismember(&sigset, SIGIO))
            printf(" SIGIO");
        if (sigismember(&sigset, SIGPWR))
            printf(" SIGPWR");
        if (sigismember(&sigset, SIGSYS))
            printf(" SIGSYS");
        if (sigismember(&sigset, SIGRTMIN))
            printf(" SIGRTMIN");
        if (sigismember(&sigset, SIGRTMIN+1))
            printf(" SIGRTMIN+1");
        if (sigismember(&sigset, SIGRTMIN+2))
            printf(" SIGRTMIN+2");
        if (sigismember(&sigset, SIGRTMIN+3))
            printf(" SIGRTMIN+3");
        if (sigismember(&sigset, SIGRTMIN+4))
            printf(" SIGRTMIN+4");
        if (sigismember(&sigset, SIGRTMIN+5))
            printf(" SIGRTMIN+5");
        if (sigismember(&sigset, SIGRTMIN+6))
            printf(" SIGRTMIN+6");
        if (sigismember(&sigset, SIGRTMIN+7))
            printf(" SIGRTMIN+7");
        if (sigismember(&sigset, SIGRTMIN+8))
            printf(" SIGRTMIN+8");
        if (sigismember(&sigset, SIGRTMIN+9))
            printf(" SIGRTMIN+9");
        if (sigismember(&sigset, SIGRTMIN+10))
            printf(" SIGRTMIN+10");
        if (sigismember(&sigset, SIGRTMIN+11))
            printf(" SIGRTMIN+11");
        if (sigismember(&sigset, SIGRTMIN+12))
            printf(" SIGRTMIN+12");
        if (sigismember(&sigset, SIGRTMIN+13))
            printf(" SIGRTMIN+13");
        if (sigismember(&sigset, SIGRTMIN+14))
            printf(" SIGRTMIN+14");
        if (sigismember(&sigset, SIGRTMIN+15))
            printf(" SIGRTMIN+15");
        if (sigismember(&sigset, SIGRTMAX-14))
            printf(" SIGRTMAX-14");
        if (sigismember(&sigset, SIGRTMAX-13))
            printf(" SIGRTMAX-13");
        if (sigismember(&sigset, SIGRTMAX-12))
            printf(" SIGRTMAX-12");
        if (sigismember(&sigset, SIGRTMAX-11))
            printf(" SIGRTMAX-11");
        if (sigismember(&sigset, SIGRTMAX-10))
            printf(" SIGRTMAX-10");
        if (sigismember(&sigset, SIGRTMAX-9))
            printf(" SIGRTMAX-9");
        if (sigismember(&sigset, SIGRTMAX-8))
            printf(" SIGRTMAX-8");
        if (sigismember(&sigset, SIGRTMAX-7))
            printf(" SIGRTMAX-7");
        if (sigismember(&sigset, SIGRTMAX-6))
            printf(" SIGRTMAX-6");
        if (sigismember(&sigset, SIGRTMAX-5))
            printf(" SIGRTMAX-5");
        if (sigismember(&sigset, SIGRTMAX-4))
            printf(" SIGRTMAX-4");
        if (sigismember(&sigset, SIGRTMAX-3))
            printf(" SIGRTMAX-3");
        if (sigismember(&sigset, SIGRTMAX-2))
            printf(" SIGRTMAX-2");
        if (sigismember(&sigset, SIGRTMAX-1))
            printf(" SIGRTMAX-1");
        if (sigismember(&sigset, SIGRTMAX))
            printf(" SIGRTMAX");

        printf("\n");
    }

    errno = errno_save;
}

int main(void)
{
    exit(0);
}