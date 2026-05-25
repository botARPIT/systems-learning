// This file demonstrate implementation of basic thread pool

#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

# define QUEUE_SIZE 5
# define NUM_WORKERS 3

pthread_mutex_t lock;
pthread_cond_t not_full;
pthread_cond_t not_empty;

typedef struct { // Struct to take any arbitary fn as an input job
    void (*function)(void*);
    void* arg;
} Job;

typedef struct {
    Job job[QUEUE_SIZE];
    int head;
    int tail;
    int count;
} JobQueue;

JobQueue queue = {
    .head = 0,
    .tail = 0,
    .count = 0
};

void enqueue(Job job){
    queue.job[queue.head] = job;
    queue.head = (queue.head + 1) % QUEUE_SIZE;
    queue.count++;
}

Job dequeue(){
    Job job = queue.job[queue.tail];
    queue.tail = (queue.tail + 1) % QUEUE_SIZE;
    queue.count--;
    return job;
}

void execute_job(void* arg){ 
    int value = *(int*)arg;

    printf("Executing the task %d\n", value);

    sleep(2);
    free(arg);
}

void submit_job(void (*function)(void*), void* arg){ // This works as a producer here and its job it to enqueue the job
    pthread_mutex_lock(&lock);
    while(queue.count == QUEUE_SIZE){
        printf("Waiting for slot in job queue \n");
        pthread_cond_wait(&not_full, &lock);
    }
    // package memory address of fn and value
    Job job = {
        .function = function,
        .arg = arg
    };

    enqueue(job);
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&lock);

}

void* worker(void* arg){
    int id = *(int*)arg;

    while(1){
        pthread_mutex_lock(&lock);

        while(queue.count == 0){
            printf("Waiting for jobs\n");
            pthread_cond_wait(&not_empty, &lock);
        }

        Job job = dequeue();
        
        // send signal
        pthread_cond_signal(&not_full);
        // release lock
        pthread_mutex_unlock(&lock);

        if(job.function == NULL){
            printf("Worker %d shutting down \n", id);
            break;
        }

        // process the job
        job.function(job.arg);
    }
    return NULL;
}

int main(){
    pthread_t worker_thread[NUM_WORKERS];
    int ids[NUM_WORKERS];
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    for(int i = 0; i < NUM_WORKERS; i++){
        ids[i] = i + 1;
        pthread_create(&worker_thread[i], NULL, worker, &ids[i]);
    }

    for (int i = 1; i <= 10; i++){
        int* value = malloc(sizeof(int));
        *value = i;
        submit_job(execute_job, value);
    }
    // inject poison pill for shutting down workers after processing all the jobs
    for (int i = 0; NUM_WORKERS > i; i++){
        submit_job(NULL, NULL);
    }
    for(int i = 0; i < NUM_WORKERS; i++){
        pthread_join(worker_thread[i], NULL);
    }

    pthread_mutex_destroy(&lock);

    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}


