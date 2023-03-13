#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

int main(void)
{
    uint32_t a = 0x04030201;
    unsigned char *cp = (unsigned char *)&a;

    if (*cp == 1) {
        printf("little-endian\n");
    } else if (*cp == 4) {
        printf("big-endian\n");
    } else {
        printf("unknown\n");
    }
    exit(0);
}