#include "apue.h"

#define MAX_LEN 4096

int my_dup2(int fd, int fd2)
{
    if (fd2 > sysconf(_SC_OPEN_MAX) || fd2 < 0) {
        printf("Invalid fd2:%d\n", fd2);
        return -1;
    }

    if (fd == fd2) {
        return fd2;
    }

    int s[MAX_LEN];
    int tail = 0;
    int n;
    while ((n = dup(fd)) < fd2)
    {
        if (-1 == n) {
            printf("System can't make file.\n");
            return -1;
        }
        s[tail++] = n;
    }
    // 执行完之后n >= fd2，这种情况下fd2一定是打开状态，所以关闭fd2再dup返回的描述符就是fd2了
    close(fd2);

    if (-1 == dup(fd))
    {
        printf("dup function error\n");
        return -1;
    }

    size_t i = 0;
    for (i = 0; i < tail; i++)
    {
        close(s[i]);
    }
    return fd2;
}

int main(int argc, char *argv[])
{
    int fd = my_dup2(STDOUT_FILENO, 7);
    write(7, "Hello World!\n", 13);
    return 0;
}