#include "apue.h"
#include <stdio.h>

// 1GB
// #define BUFFSIZE    1073741824
// #define BUFFSIZE    1048576
// #define BUFFSIZE    1024
#define BUFFSIZE    8000000

void my_setbuf(FILE *restrict fp, char *restrict buf)
{
    int len = BUFFSIZE;
    if (NULL == buf || stderr == fp) {
        if (setvbuf(fp, NULL, _IONBF, 0) != 0) {
            err_sys("setbuf _IONBF error");
        }
        return;
    }

    int file = fileno(fp);
    struct stat st;
    if (fstat(file, &st) == -1) {
        err_sys("fstat error");
    } else if (S_ISFIFO(st.st_mode) || S_ISSOCK(st.st_mode) || S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode)) {
        if (setvbuf(fp, buf, _IOLBF, len) != 0)
            err_sys("setbuf _IOLBF error");
    } else {
        if (setvbuf(fp, buf, _IOFBF, len) != 0)
            err_sys("setbuf _IOFBF error");
    }
}

void sig_alrm(int signo)
{
    printf("in sig_alrm\n");
}

int main(void)
{
    char buf[BUFFSIZE];
    FILE *fp = fopen("foo", "w");
    my_setbuf(fp, buf);

    if (signal(SIGALRM, sig_alrm) == SIG_ERR) {
        err_sys("signal(SIG_ALRM) error\n");
        exit(0);
    }

    alarm(1);

    fwrite(&buf, sizeof(buf), 2, fp);

    fclose(fp);

    pause();

    exit(0);
}