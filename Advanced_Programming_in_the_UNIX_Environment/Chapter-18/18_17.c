#include <signal.h>
#include <stdio.h>
#include <termios.h>

#define MAX_PASS_LEN    8

char *getpass(const char *prompt)
{
    static char     buf[MAX_PASS_LEN+1];
    char            *ptr;
    FILE            *fp;
    int             c;
    sigset_t        sig, osig;
    struct termios  ts, ots;

    if ((fp = fopen(ctermid(NULL), "r+")) == NULL)
        return(NULL);
    setbuf(fp, NULL);

    sigemptyset(&sig);
    sigaddset(&sig, SIGINT);
    sigaddset(&sig, SIGTSTP);
    sigprocmask(SIG_BLOCK, &sig, &osig);

    tcgetattr(fileno(fp), &ts);
    ots = ts;
    ts.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
    tcsetattr(fileno(fp), TCSAFLUSH, &ts);
    fputs(prompt, fp);

    ptr = buf;
    while ((c = getc(fp)) != EOF && c != '\n')
    {
        if (ptr < &buf[MAX_PASS_LEN])
            *ptr++ = c;
    }
    *ptr = 0;
    putc('\n', fp);
    
    tcsetattr(fileno(fp), TCSAFLUSH, &ots);
    sigprocmask(SIG_SETMASK, &osig, NULL);
    fclose(fp);
    return(buf);
}