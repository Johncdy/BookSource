#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        err_quit("usage: ./a.out file1 file2\n");
        exit(1);
    }

    int fd1, fd2;
    if ((fd1 = open(argv[1], O_RDWR)) == -1) {
        printf("open %s failed!\n", argv[1]);
        return -1;
    }

    if ((fd2 = open(argv[2], O_RDWR | O_CREAT, 0666)) == -1) {
        printf("open %s failed!\n", argv[2]);
        return -1;
    }

    char buf;
    while (read(fd1, &buf, 1) == 1)
    {
        if (buf != 0) {
            if (write(fd2, &buf, 1) != 1) {
                printf("write %s error", argv[2]);
                return -1;
            }
        }
    }
    
    return 0;
}