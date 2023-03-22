#include <termios.h>
#include "apue.h"

int isatty(int fd)
{
    struct termios ts;
    return(tcgetattr(fd, &ts) != -1);
}