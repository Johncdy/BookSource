#include "apue.h"
#include <fcntl.h>
#include <limits.h>

void set_fl(int fd, int flags)
{
    int val;

    if ((val = fcntl(fd, F_GETFL, 0)) < 0)
        err_sys("fcntl F_GETFL error");

    val |= flags;

    if (fcntl(fd, F_SETFL, val) < 0)
        err_sys("fcntl F_SETFL error");
}

int main(void)
{
    int i, n;
    int fd[2];

    if (pipe(fd) < 0)
        err_sys("pipe error");
    set_fl(fd[1], O_NONBLOCK);

    for (n = 0; ; n++)
    {
        if ((i = write(fd[1], "a", 1)) != 1) {
            printf("write ret %d, ", i);
            break;
        }
    }
    
    printf("pipe capacity = %d\n", n);
    printf("PIPE_BUF = %d\n", _POSIX_PIPE_BUF);
    printf("PIPE_SIZE = %d\n", _POSIX_PIPE_SIZE);

    exit(0);
}