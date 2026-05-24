#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

# define QUEUE_SIZE 5
# define NUM_WORKERS 3

typedef struct {
    int jobs[QUEUE_SIZE];
    int front;
    int rear;
    int count;
} JobQueue;

JobQueue queue = {
    .front = 0,
    .rear = 0,
    .count = 0
};

pthread_mutex_t lock;

pthread_cond_t not_empty;
pthread_cond_t not_full;

void enqueue(int job){
    queue.jobs[queue.rear] = job;
    queue.rear = (queue.rear + 1) % QUEUE_SIZE;
    queue.count++;
}

int dequeue(){
    int job = queue.jobs[queue.front];
    queue.front = (queue.front + 1) % QUEUE_SIZE;
    queue.count--;

    return job;
}

void* worker(void* arg){
    int id = *(int*)arg;
    while(1){
        pthread_mutex_lock(&lock);

        while(queue.count == 0){
            printf("Worker %d waiting for jobs \n", id);
            pthread_cond_wait(&not_empty, &lock);
        }

        int job = dequeue();
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&lock);

        printf("Worker %d processed job %d\n", id, job);
        sleep(2);
    }

    return NULL;
}

void* producer(void* arg){
    for (int i = 1; i <= 10; i++){
        pthread_mutex_lock(&lock);
        while(queue.count == QUEUE_SIZE){
            printf("Queue full producer waiting \n");
            pthread_cond_wait(&not_full, &lock);
        }

        enqueue(i);
        printf("Submitted job %d\n", i);
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&lock);
        
        sleep(1);
        
    }

    return NULL;
}
int main(){

    pthread_t workers[NUM_WORKERS];
    pthread_t producer_thread;

    int ids[NUM_WORKERS];

    pthread_mutex_init(&lock, NULL);

    pthread_cond_init(&not_empty, NULL);
    pthread_cond_init(&not_full, NULL);

    // Create worker threads
    for(int i = 0; i < NUM_WORKERS; i++){

        ids[i] = i + 1;
        pthread_create(&workers[i], NULL, worker, &ids[i]);
    }

    pthread_create(&producer_thread, NULL, producer, NULL);

    pthread_join(producer_thread, NULL);

    sleep(10);
    return 0;
}