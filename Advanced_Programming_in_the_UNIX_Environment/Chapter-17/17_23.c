#include "apue.h"

#define MAXARGC     50
#define WHITE   " \t\n"

int buf_args(char *buf, int (*optfunc)(int, char **))
{
    char *ptr, *argv[MAXARGC];
    int argc;

    if (strtok(buf, WHITE) == NULL)
        return(-1);
    argv[argc = 0] = buf;
    while ((ptr = strtok(NULL, WHITE)) != NULL)
    {
        if (++argc >= MAXARGC-1)
            return(-1);
        argv[argc] = ptr;
    }
    argv[++argc] = NULL;
    
    return((*optfunc)(argc, argv));
}