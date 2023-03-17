#include "open.h"
#include <sys/uio.h>

int fd_pipe(int fd[2])
{
    return(socketpair(AF_UNIX, SOCK_STREAM, 0, fd));
}

int csopen(char *name, int oflag)
{
    pid_t pid;
    int len;
    char buf[10];
    struct iovec iov[3];
    static int fd[2] = { -1, -1 };

    if (fd[0] < 0) {
        if (fd_pipe(fd) < 0) {
            err_ret("fd_pipe error");
            return(-1);
        }
        if ((pid = fork()) < 0) {
            err_ret("fork error");
            return(-1);
        } else if (pid == 0) {
            close(fd[0]);
            if (fd[1] != STDIN_FILENO && )
        }
    }
}