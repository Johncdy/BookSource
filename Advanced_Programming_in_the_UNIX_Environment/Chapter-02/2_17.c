#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/resource.h>

#define OPEN_MAX_GUESS 256

long open_max(void)
{
    long openmax;
    struct rlimit rl;

    if (openmax == 0) {
        errno = 0;

        if ((openmax = sysconf(_SC_OPEN_MAX)) < 0 || openmax == LONG_MAX) {
            if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
                perror("can't get file limit");
            } else if (rl.rlim_max == RLIM_INFINITY) {
                openmax = OPEN_MAX_GUESS;
            } else {
                openmax = rl.rlim_max;
            }
        }
    }
    return(openmax);
}

int main(void)
{
    printf("openmax:%ld\n", open_max());
}