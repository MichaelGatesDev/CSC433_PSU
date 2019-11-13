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

// goal is to allow multiple readers at the same time, but
// if the writer has it, then it can not be shared
// if the writer requests the lock, then he shouldn't be starved

pthread_cond_t rw_cond;
pthread_mutex_t rw_lock; // this protects the rw_lock data struture
int reader_count = 0;
int writer_flag = 0;

pthread_mutex_t lock;

void reader_lock () {
    pthread_mutex_lock(&rw_lock);

    while(writer_flag > 0){
        pthread_cond_wait(&rw_cond, &rw_lock);
    }

    if(reader_count == 0){
        pthread_mutex_lock(&lock);
    }
    reader_count++;
    pthread_mutex_unlock(&rw_lock);
    // printf("num readers is %d\n", reader_count);
}
 
void writer_lock () {
    // printf("writer attempting to grab lock\n");
    writer_flag++;
    pthread_mutex_lock(&lock);
    // printf("writer got lock\n");
}
 
void reader_unlock () {
    pthread_mutex_lock(&rw_lock);
    reader_count--;
    if(reader_count == 0){
        pthread_mutex_unlock(&lock);
    }
    pthread_mutex_unlock(&rw_lock);
}

void writer_unlock () {
    writer_flag--;
    pthread_cond_broadcast (&rw_cond);
    pthread_mutex_unlock(&lock);
}


void *reader (void *arg){
    int *id = (int*) arg;
    // printf("reader thread %d\n", id);
    int y;
    // all the values in the array should be the same if everything is correct
    // count the errors
    while(y == y){ 
        reader_lock();
        y = shared_data[0];
        for (int i = 0; i < DATASIZE; i++)
        {
            if(shared_data[i] != y){
                reader_errors[*id] += 1;
            }
        }
        reader_unlock();
        // sleep(1);
    }
}

void *writer (void *arg){
    int count =0;
    int y;
    for (int count = 0; count < WRITECOUNT; count++)
    {
        y = 0;
        writer_lock();
        for (int i = 0; i < DATASIZE; i++)
        {
            shared_data[i] += 5;
        }
        writer_unlock();
    }
    
}

void main() {

    long nanos;
    struct timespec start_time, end_time;
    clock_gettime (CLOCK_PROCESS_CPUTIME_ID, &start_time);

    pthread_mutex_init (&lock, NULL);
    pthread_mutex_init (&rw_lock, NULL);
    pthread_cond_init (&rw_cond, NULL);

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