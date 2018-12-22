#include <stdio.h>
#include <stdlib.h>

typedef struct {
    unsigned long lcm, gcd;
} result_t;

result_t lcm_gcd(unsigned long x, unsigned long y);

int main(int argc, char **argv) {
    if (argc < 3)
    {
        return 1;
    }

    unsigned long x = strtol(argv[1], NULL, 10);
    unsigned long y = strtol(argv[2], NULL, 10);
    result_t struktura = lcm_gcd(x,y);

    printf("%ld %ld\n", struktura.gcd, struktura.lcm);

    return 0;
}
