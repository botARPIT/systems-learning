// Writing the counter program with using mutexes

#include <stdio.h>
#include <pthread.h>
#include <time.h>
int counter = 0;


// Creating mutex object
pthread_mutex_t lock;
void* increment(void* arg){
    int local_counter = 0;
    for (int i = 0; i < 1000000; i++){
        // pthread_mutex_lock(&lock);
        local_counter++;
        // pthread_mutex_unlock(&lock);
    }
    // Update the global counter once
    pthread_mutex_lock(&lock);
    counter += local_counter;
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(){

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    pthread_t t1;
    pthread_t t2;

    // Initialise mutexes
    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Value of counter: %d\n", counter);
    pthread_mutex_destroy(&lock);
    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Execution time taken: %f\n", time_taken);
    return 0;
}
