#include <stdio.h>
#include <stdlib.h>

int main() {

    int *p = malloc(sizeof(int));

    *p = 50;

    printf("Before free: %d\n", *p);

    free(p);

    printf("After free: %d\n", *p);

    return 0;
}