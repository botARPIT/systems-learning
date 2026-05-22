#include <stdio.h>
#include <stdlib.h>

int main() {

    int *p = malloc(sizeof(int));

    *p = 50;

    printf("Value: %d\n", *p);

    printf("Address stored in p: %p\n", (void*)p);

    free(p);

    return 0;
}