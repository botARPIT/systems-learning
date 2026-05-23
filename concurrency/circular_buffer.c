#include<stdio.h>
#include<stdbool.h>

#define CAPACITY 4

typedef struct {
    int buffer[CAPACITY];
    int head;
    int tail;
    int count;
}CircularBuffer;

bool cb_write(CircularBuffer *cb, int val){
    if(cb->count == CAPACITY){
        return false;
    }

    cb->buffer[cb->head] = val;
    cb->head = (cb->head + 1) % CAPACITY;
    cb->count++;
    return true;
}

bool cb_read(CircularBuffer *cb, int *val){
    if(cb->count == 0){
        return false; // Empty buffer
    }

    *val = cb->buffer[cb->tail]; // Copy the value from buffer to memory address of caller fn
    cb->tail = (cb->tail + 1) % CAPACITY;
    cb->count--;
    return true;

    
}


int main(){
    // Initialize the circular buffer 
    CircularBuffer cb = {.head = 0, .tail = 0, .count = 0};

    // Write data in buffer until full
    int values_to_write[] = {10, 20, 30, 40, 50};

    for (int i = 0; i <= CAPACITY; i++){
        bool write_success = cb_write(&cb, values_to_write[i]);

        if(write_success){
            printf("Wrote the value: %d, count is: %d and the head is at: %d\n", 
            values_to_write[i], cb.count, cb.head);
        } else {
            printf("Unable to write -> Buffer full. Count is: %d and head is at: %d\n",
            cb.count, cb.head);
        }

    }

    // Reading data from the buffer
    int val = 0;

    for (int i = 0; i <= CAPACITY; i++){
        bool read_success = cb_read(&cb, &val);

        if(read_success){
            printf("Successfully read the data: %d, count is: %d, and the tail is at: %d\n",
            val, cb.count, cb.tail);
        } else {
            printf("Failed to read -> buffer is empty. Count is: %d\n",
            cb.count);
        }
    }

    return 0;
}
