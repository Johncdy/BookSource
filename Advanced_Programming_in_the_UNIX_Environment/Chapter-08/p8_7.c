#include "apue.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <dirent.h>

int main(void)
{
    DIR *dir;
    int fd;
    int fd_flags;
    
    if ((dir = opendir("/")) == NULL)
        err_sys("opendir error");
    
    if ((fd = dirfd(dir)) < 0)
        err_sys("dirfd error");
    
    if ((fd_flags = fcntl(fd, F_GETFD, 0)) < 0)
        err_sys("fcntl error");
    
    if (fd_flags & FD_CLOEXEC)
        printf("opendir close-on-exec is on\n");
    else
        printf("opendir close-on-exec is off\n");
    
    if ((fd = open("/", O_DIRECTORY)) < 0)
        err_sys("open error");
    
    if ((fd_flags = fcntl(fd, F_GETFD, 0)) < 0)
        err_sys("fcntl error");

    if (fd_flags & FD_CLOEXEC)
        printf("open close-on-exec is on\n");
    else
        printf("open close-on-exec is off\n");
    
    exit(0);
}