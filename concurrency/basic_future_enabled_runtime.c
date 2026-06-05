// This file demonstrates use of future object to store the variable result after computation on 
// seperate thread and return result to main thread

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define QUEUE_SIZE 4
pthread_mutex_t lock;
pthread_cond_t not_full;
pthread_cond_t not_empty;


typedef struct {
    int result;
    int completed;
    pthread_mutex_t fut_lock;
    pthread_cond_t res_update;
} Future;

typedef struct {
    int input;
    Future* future;
} Job;

typedef struct {
    Job jobs[QUEUE_SIZE];
    int head;
    int tail;
    int count;
} JobQueue;


JobQueue queue = {
    .head = 0,
    .tail = 0,
    .count = 0,
};

void enqueue(Job job){
    queue.jobs[queue.head] = job;
    queue.head = (queue.head + 1) % QUEUE_SIZE;
    queue.count++; 
}

Job dequeue(){
    Job job = queue.jobs[queue.tail];
    queue.tail = (queue.tail + 1) % QUEUE_SIZE;
    queue.count--;
    return job;
}

void* produce(void* arg){
    Job* input_job = (Job*)arg;

    pthread_mutex_lock(&lock);
    while(queue.count == QUEUE_SIZE){
        printf("Queue full. Waiting...\n");
        pthread_cond_wait(&not_full, &lock);
    }

    enqueue(*input_job);
    printf("Job enqueued \n");
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&not_empty);

    return NULL;

}

void* consume(void* arg){
    pthread_mutex_lock(&lock);
    while(queue.count == 0){
        printf("No jobs to process. Waiting...\n");
        pthread_cond_wait(&not_empty, &lock);
    }

    Job job = dequeue();
    pthread_mutex_unlock(&lock);
    pthread_cond_signal(&not_full);
    sleep(5);
    int square = job.input * job.input;

    pthread_mutex_lock(&job.future->fut_lock);
    job.future->result = square;
    job.future->completed = 1;
    pthread_cond_signal(&job.future->res_update);
    pthread_mutex_unlock(&job.future->fut_lock);
    return NULL;
}


int main(){
    pthread_t producer_thread, consumer_thread;
    Future* my_future = (Future*)malloc(sizeof(Future));
    my_future->result = 0;
    my_future->completed = 0;
    pthread_mutex_init(&my_future->fut_lock, NULL);
    pthread_cond_init(&my_future->res_update, NULL);

    Job local_job = {
        .input = 20,
        .future = my_future
    };

    pthread_create(&producer_thread, NULL, produce, &local_job);
    pthread_create(&consumer_thread, NULL, consume, NULL);
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    pthread_mutex_lock(&my_future->fut_lock);
    while(!my_future->completed){
        printf("Result not ready. Waiting...\n");
        pthread_cond_wait(&my_future->res_update, &my_future->fut_lock);


    }
    printf("The result is: %d\n", my_future->result);
    pthread_mutex_unlock(&my_future->fut_lock);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&lock);
    pthread_mutex_destroy(&my_future->fut_lock);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);
    pthread_cond_destroy(&my_future->res_update);
    free(my_future);
    return 0;
}
