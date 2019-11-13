#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define DATASIZE 1000000
#define NUMREADERS 10
#define NUMWRITERS 1
#define WRITECOUNT 100 // how many writes before the program ends

int shared_data[DATASIZE];
int reader_errors[NUMREADERS];

pthread_mutex_t lock;

void *reader (void *arg){
    int *id = (int*) arg;
    // printf("reader thread %d\n", id);
    int y;
    // all the values in the array should be the same if everything is correct
    // cuont the errors
    while(y == y){
        pthread_mutex_lock(&lock);
        y = shared_data[0];
        for (int i = 0; i < DATASIZE; i++)
        {
            if(shared_data[i] != y){
                reader_errors[*id] += 1;
            }
        }
        pthread_mutex_unlock(&lock);
        // sleep(1);
    }
}

void *writer (void *arg){
    int count =0;
    pthread_mutex_lock(&lock);
    for (int count = 0; count < WRITECOUNT; count++)
    {
        int y;
        for (int i = 0; i < DATASIZE; i++)
        {
            // shared_data[i] += 5;
        }
        pthread_mutex_unlock(&lock);
    }
    
}

void main() {

    long nanos;
    struct timespec start_time, end_time;
    clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start_time);


    pthread_mutex_init (&lock, NULL);



    for(int i=0; i<DATASIZE; i++){
        shared_data[i] = 10;
    }

    pthread_t readers[NUMREADERS], writers[NUMWRITERS];
    int ids[NUMREADERS];

    // make reader threads
    for(int i=0; i<NUMREADERS; i++){
        ids[i] = i;
        reader_errors[i] = 0;
        pthread_create(&readers[i], NULL, reader, (void*)&(ids[i]));
    }

    // make writer threads
    for(int i=0; i<NUMWRITERS; i++){
        pthread_create(&writers[i], NULL, writer, NULL);
    }

    // only join on the writer s
    for(int i=0; i<NUMWRITERS; i++){
        pthread_join(writers[i], NULL);
    }

    

    clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &end_time);

    nanos = (end_time.tv_sec - start_time.tv_sec) * 1e9 + (end_time.tv_nsec - start_time.tv_nsec);

    int sum = 0;
    for(int i=0;i<NUMREADERS; i++){
        sum += reader_errors[i];
    }

    printf("This program took %ld nanoseconds, num errors %d\n", nanos, sum);
}