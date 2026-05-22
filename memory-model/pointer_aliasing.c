#include <stdio.h>

int main() {

    int x = 10;

    int *p1 = &x;
    int *p2 = &x;

    printf("Initial x: %d\n", x);

    *p1 = 50;

    printf("Value via p2: %d\n", *p2);

    printf("Final x: %d\n", x);

    return 0;
}