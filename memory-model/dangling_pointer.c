#include <stdio.h>

int* dangerous() {

    int x = 42;

    printf("x address inside function: %p\n", (void*)&x);

    return &x;
}

int main() {

    int *ptr = dangerous();

    printf("Pointer value: %p\n", (void*)ptr);

    printf("Dereferenced value: %d\n", *ptr);

    return 0;
}