#include "apue.h"
#include <time.h>

int main(void)
{
    time_t t;
    struct tm *tmp;
    char buf[64];

    time(&t);
    tmp = localtime(&t);
    if (strftime(buf, 64, "%a %b %d %X %Z %Y\n", tmp) == 0)
        err_sys("strftime error");
    fputs(buf, stdout);

    exit(0);
}