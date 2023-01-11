#include "apue.h"
#include <fcntl.h>

static volatile sig_atomic_t sigflag;
static sigset_t newmask, oldmask, zeromask;

static void sig_usr(int signo)
{
    sigflag = 1;
}

void TELL_WAIT(void)
{
    if (signal(SIGUSR1, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR1) error");
    
    if (signal(SIGUSR2, sig_usr) == SIG_ERR)
        err_sys("signal(SIGUSR2) error");
    
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        err_sys("SIG_BLOCK error");
}

void TELL_PARENT(pid_t pid)
{
    kill(pid, SIGUSR2);
}

void WAIT_PARENT(void)
{
    while (sigflag == 0)
        sigsuspend(&zeromask);
    sigflag = 0;

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK, error");
}

void TELL_CHILD(pid_t pid)
{
    kill(pid, SIGUSR1);
}

void WAIT_CHILD(void)
{
    while (sigflag == 0)
        sigsuspend(&zeromask);
    
    sigflag = 0;
    
    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        err_sys("SIG_SETMASK error");
}

volatile int count = 0;

void writenum()
{
    FILE *file = fopen("foo", "w");
    fprintf(file, "%d\n", count);
    count++;
    fclose(file);
}

int main(void)
{
    int fd;
    if ((fd = creat("foo", FILE_MODE)) < 0)
    {
        err_sys("create error for foo");
        exit(0);
    }
    
    writenum();

    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        while (1)
        {
            TELL_WAIT();
            writenum();
            printf("Child write %d\n", count);
            TELL_PARENT(getpid());
            WAIT_PARENT();
        }
    } else {
        while (1)
        {
            TELL_WAIT();
            WAIT_CHILD();
            writenum();
            printf("Parent write %d\n", count);
            TELL_CHILD(pid);
        }
    }

    exit(0);
}