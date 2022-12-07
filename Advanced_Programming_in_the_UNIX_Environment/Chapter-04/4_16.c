#include "apue.h"
#include <fcntl.h>

int main(void)
{
    if (open("foo", O_RDWR) < 0)
        err_sys("open error");
    if (unlink("foo") < 0)
        err_sys("unlink error");
    printf("file unlinked\n");
    sleep(15);
    printf("done\n");
    exit(0);
}