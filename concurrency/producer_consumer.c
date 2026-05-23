// This file is used to demonstrate basics of producer-consumer systems

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

// Define the data and ready flag

int data = 0;
int data_ready = 0;

pthread_mutex_t lock;
pthread_cond_t cond;

void* producer(void* arg){
    sleep(5);

    pthread_mutex_lock(&lock);
    data = 4452;
    data_ready = 1;

    printf("Producer produced data \n");

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

    return NULL;

}

void* consumer(void* arg){
    pthread_mutex_lock(&lock);

    while(data_ready == 0){
        printf("Consumer waiting... \n");
        pthread_cond_wait(&cond, &lock);
    }

    printf("Consumer received data: %d\n", data);
    pthread_mutex_unlock(&lock);

    return NULL;
}
int main(){
    pthread_t p; // producer thread
    pthread_t c; // consumer thread

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);

    pthread_join(p, NULL);
    pthread_join(c, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}