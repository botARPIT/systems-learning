#include<unistd.h>
#include<stdio.h>

int global_counter = 100;

int main(){

    printf("Before fork: %d\n", global_counter);
    pid_t pid = fork();

    if(pid == 0){
        global_counter = 999;
        printf("Child process sees global_counter as: %d, and address is: %p\n", global_counter, &global_counter);

    } else {

        sleep(2);
        printf("Parent process sees: %d, and address is: %p\n", global_counter, &global_counter);
    }

    return 0;
}