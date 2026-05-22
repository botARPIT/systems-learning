// This file prints the memory addresses of variables

#include <stdio.h>

void test(){
    int a = 20;
    int c = 40;

    printf("Value of variable a: %d\n", a);
    printf("Memory address of variable a: %p\n", (void*)&a);

    printf("Value of variable c: %d\n", c);
    printf("Memory address of variable c: %p\n", (void*)&c);
}

int main(){
    int b = 200;

    test();

    printf("Value of variable b: %d\n", b);
    printf("Memory address of variable b: %p\n", (void*)&b);

    return 0;
}