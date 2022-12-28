#include "apue.h"
#include <sys/wait.h>

void mypr_exit(siginfo_t *infop)
{
    if (infop->si_code == CLD_EXITED)
        printf("normal termination, exit status = %d\n", infop->si_status);
    else if (infop->si_code == CLD_KILLED)
        printf("abnormal termaination, signal number = %d%s\n", infop->si_status, infop->si_status == CLD_DUMPED ? "(core file generated)" : "");
    else if (infop->si_code == CLD_STOPPED)
        printf("child stoped, signal number = %d\n", infop->si_status);
}

int main(void)
{
    pid_t pid;
    int status;
    siginfo_t infop;

    if ((pid = fork()) < 0)
        printf("fork error");
    else if (pid == 0)
        exit(7);
    
    if (waitid(P_ALL, 0, &infop, WEXITED) != 0)
        err_sys("waitid error");
    mypr_exit(&infop);

    if ((pid = fork()) < 0)
        err_sys("fork error");
    else if (pid == 0)
        abort();
    
    if (waitid(P_ALL, 0, &infop, WEXITED) != 0)
        err_sys("waitid error");
    else if (pid == 0)
        abort();
    mypr_exit(&infop);

    if ((pid = fork()) < 0)
        err_sys("wait error");
    else if (pid == 0)
        status /= 0;
    
    if (waitid(P_ALL, 0, &infop, WEXITED) != 0)
        err_sys("waitid error");
    mypr_exit(&infop);

    exit(0);
}