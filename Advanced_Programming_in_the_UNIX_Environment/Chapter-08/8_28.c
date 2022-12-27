#include "apue.h"

int main(void)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {
        sleep(2);
        exit(1);
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {
        sleep(4);
        abort();
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {
        execl("/bin/dd", "dd", "if=/etc/passwd", "of=/dev/null", (char*)0);
        exit(7);
    }

    if ((pid = fork()) < 0) {
        err_sys("fork error");
    } else if (pid > 0) {
        sleep(8);
        exit(0);
    }

    sleep(6);
    kill(getpid(), SIGKILL);
    exit(6);
}