#include "apue.h"
#include <stdio.h>

void pr_studio(const char *, FILE *);
int is_unbuffered(FILE *);
int is_linebuffered(FILE *);
int buffer_size(FILE *);
void my_setbuf(FILE *restrict fp, char *restrict buf);

int main(void)
{
    FILE *fp = fopen("test", "w");
    char buf[BUFSIZ];
    pr_studio("fp", fp);
    my_setbuf(fp, NULL);
    pr_studio("fp", fp);
    my_setbuf(fp, buf);
    pr_studio("fp", fp);
    pr_studio("stderr", stderr);
    my_setbuf(stderr, buf);
    pr_studio("stderr", stderr);
    pr_studio("stdin", stdin);
    my_setbuf(stdin, buf);
    pr_studio("stdin", stdin);
    pr_studio("stdout", stdout);
    my_setbuf(stdout, buf);
    pr_studio("stdout", stdout);

    return 0;
}

void my_setbuf(FILE *restrict fp, char *restrict buf)
{
    int len = BUFSIZ;
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

void pr_studio(const char *name, FILE *fp)
{
    printf("stream = %s, ", name);
    if (is_unbuffered(fp))
        printf("unbuffered");
    else if (is_linebuffered(fp))
        printf("line buffered");
    else
        printf("full buffered");
    printf(", buffer size = %d\n", buffer_size(fp));
}

int is_unbuffered(FILE *fp)
{
    return (fp->_flags & _IO_UNBUFFERED);
}

int is_linebuffered(FILE *fp)
{
    return (fp->_flags & _IO_LINE_BUF);
}

int buffer_size(FILE *fp)
{
    return (fp->_IO_buf_end - fp->_IO_buf_base);
}