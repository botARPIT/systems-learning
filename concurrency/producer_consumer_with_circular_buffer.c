// This file demonstrate how producer-consumer is implemented using circular buffers with backpressure

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

# define BUFFER_SIZE 4 

// Define circular buffer
typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    int count;
} CircularBuffer;

// Define lock
pthread_mutex_t lock;

// Define kernel managed wait queue
pthread_cond_t not_full;
pthread_cond_t not_empty;

void* producer(void* arg){
    CircularBuffer *cb = (CircularBuffer*)arg;
    for(int i = 0; i <= 10; i++){
        pthread_mutex_lock(&lock);

        while (cb->count == BUFFER_SIZE){
            printf("Producer waiting..., buffer full \n");
            pthread_cond_wait(&not_full, &lock);
        }

        cb->buffer[cb->in] = i;
        printf("Produced \n");

        cb->in = (cb->in + 1) % BUFFER_SIZE;

        cb->count++;

        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* consumer(void* arg){
    CircularBuffer *cb = (CircularBuffer*)arg;
    for(int i = 0; i <= 10; i++){
        pthread_mutex_lock(&lock);
        while(cb->count == 0){
            printf("No work left for consumer \n");
            pthread_cond_wait(&not_empty, &lock);
        }

        int item = cb->buffer[cb->out];
        printf("Consumed: %d\n", item);
        cb->out = (cb->out + 1) % BUFFER_SIZE;
        cb->count--;

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);


    }
    return NULL;
}

int main(){

    // Initialise circular buffer

    CircularBuffer cb = {.in = 0, .out = 0, .count = 0};

    // Define respective threads
    pthread_t producer_thread;
    pthread_t consumer_thread;

    // Initialise lock
    pthread_mutex_init(&lock, NULL);

    // Initialise kernel managed wait queue
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    // Initialize the threads
    pthread_create(&producer_thread, NULL, producer, &cb);
    pthread_create(&consumer_thread, NULL, consumer, &cb);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&lock);

    return 0;
}

