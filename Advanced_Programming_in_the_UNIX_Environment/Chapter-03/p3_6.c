#include "apue.h"
#include <fcntl.h>

int main(void)
{
    int fd;

    if ((fd = open("tmp", O_RDWR | O_APPEND)) == -1) {
        printf("open failed!\n");
        return 0;
    }

    if (write(fd, "Hello", 5) == -1) {
        printf("write failed!\n");
        return 0;
    }

    int r;
    if ((r = lseek(fd, 3, SEEK_SET)) == -1) {
        printf("lseek failed!\n");
        close(fd);
        return 0;
    }

    if (write(fd, " World!\n", 8) == -1) {
        printf("write failed!\n");
        return 0;
    }
    close(fd);

    if ((fd = open("tmp", O_RDWR | O_APPEND)) == -1) {
        printf("open failed!\n");
        return 0;
    }

    if ((r = lseek(fd, 6, SEEK_SET)) == -1) {
        printf("lseek failed!\n");
        close(fd);
        return 0;
    }

    char str[32];
    read(fd, str, 15);
    printf("%s\n", str);
    close(fd);

    return 0;
}