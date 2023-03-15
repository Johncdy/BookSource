#include "open.h"
#include <fcntl.h>

#define BUFFSIZE    8192

int main(int argc, char *argv[])
{
    int n, fd;
    char buf[BUFFSIZE];
    char line[BUFFSIZE];

    while (fgets(line, MAXLINE, stdin) != NULL)
    {
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '0';
        if ((fd = csopen(line, O_RONLY)) < 0)
            continue;

        
    }
    
}