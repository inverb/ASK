#include <stdio.h>
#include <stdlib.h>

int clz(long x);

int main(int argc, char **argv) {
    if (argc < 2)
    {
        return 1;
    }

    long x = strtol(argv[1], NULL, 10);

    printf("%d\n", clz(x));

    return 0;
}
