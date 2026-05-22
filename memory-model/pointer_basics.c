// This file contains pointer basics

#include<stdio.h>

int main(){
    int a = 30;

    printf("Current value of variable a: %d\n", a);
    printf("Memory address of variable a: %p\n", (void*)&a);
    int *p = &a; // Declaration and initialization of pointer variable

    // Print the value stored in p
    printf("Value stored in pointer variable p: %p\n", (void*)p);

    // Print the memory address of pointer variable
    printf("Memory address of pointer variable p : %p\n", (void*)&p);

    // Deference pointer variable
    printf("Dereference pointer variable p: %d\n", *p);

    // Updating the value of a using pointer variable 
    *p = 400;

    // Print new value of variable a
    printf("New value of variable a: %d\n", a); 

    return 0;
}