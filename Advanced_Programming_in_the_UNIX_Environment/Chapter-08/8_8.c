#include "apue.h"

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid == 0) {
        printf("first child exec\n");
        if ((pid = fork()) < 0) {
            err_sys("fork error");
        } else if (pid > 0) {
            exit(0);
        }

        sleep(2);
        printf("second child, parent pid = %ld\n", (long)getppid());
        exit(0);
    }

    printf("main exec\n");

    if (waitpid(pid, NULL, 0) != pid)
        err_sys("waitpid error");

    exit(0);
}