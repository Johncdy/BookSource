#include "apue.h"
#include <pthread.h>

void cleanup(void *arg)
{
    printf("cleanup: %s\n", (char *)arg);
}

