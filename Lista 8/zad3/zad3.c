#include <stdio.h>
#include <stdlib.h>

long tab[1000009];

void insert_sort(long *first, long *last);

int main(int argc, char **argv) {
    if (argc < 1)
    {
        return 1;
    }
    
    for(int i=1; i<argc; i++)
    {
        long x = strtol(argv[i], NULL, 10);
        tab[i-1]=x;
    }
    long* it = &tab[0];
    insert_sort(it, it+argc-2);
    for(int i=1; i<argc; i++) printf("%ld ", tab[i-1]);
    printf("\n");
    return 0;
}
