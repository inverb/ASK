#include <stdio.h>
#include <stdlib.h>

unsigned long fibonacci(unsigned long n);

int main(int argc, char **argv) {
    if (argc < 2)
    {
        return 1;
    }

    unsigned long n = strtol(argv[1], NULL, 10);

    printf("%ld\n", fibonacci(n));
    return 0;
}
