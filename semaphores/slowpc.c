#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <pthread.h>
#include <threads.h>
#include <semaphore.h>

#define LOOPS 100000
#define PRODUCERS 1
#define CONSUMERS 20
#define MAXSIZE 100


int *buffer;
int head; // where to read values
int tail; // where we want to insert values
int size;
int notdone = 1;

sem_t empty; // how many empty spaces 
sem_t full; // how many full spaces
sem_t lock; // coordinate on modifications

void *prod_fn(void *arg){
    int count = 0;
    while(count < LOOPS){
        sem_wait(&empty);
        sem_wait(&lock);

        printf("Adding %d to buffer\n", count);
        buffer[tail] = count++;
        tail = (tail + 1) % MAXSIZE;
        size++;

        sem_post(&lock);
        sem_post(&full);
    }
    notdone = 0;
    return NULL;
}

void *cons_fn(void *arg){
    while(notdone == 1 || size > 0){

        sem_wait(&full);
        sem_wait(&lock);

        int value = buffer[head];
        printf("Got %d from the buffer\n", value);
        head = (head + 1) % MAXSIZE;
        size--;

        sem_post(&lock);
        sem_post(&empty);
    }
    return NULL;
}


void main(){

    buffer = (int*) malloc(MAXSIZE * sizeof(int));

    int i;
    for (i = 0; i < MAXSIZE; i++)
    {
        buffer[i] = 0;
    }
    size = 0;
    head = 0;
    tail = 0;

    sem_init(&empty, 0, MAXSIZE);
    sem_init(&full, 0, 0);
    sem_init(&lock, 0, 1);


    // create threads 

    pthread_t producers[PRODUCERS];
    for(i=0; i<PRODUCERS; i++){
        pthread_create(&producers[i], NULL, prod_fn, NULL);
    }

    pthread_t consumers[CONSUMERS];
    for(i=0; i<CONSUMERS; i++){
        pthread_create(&consumers[i], NULL, cons_fn, NULL);
    }


    // wait for threads to finish
    for(i=0; i<PRODUCERS; i++){
        pthread_join(producers[i], NULL);
    }


    for(i=0; i<CONSUMERS; i++){
        sem_post(&full);
    }


    for(i=0; i<CONSUMERS; i++){
        pthread_join(consumers[i], NULL);
    }

    for (i = 0; i < MAXSIZE; i++)
    {
        printf("%d ", buffer[i]);
    }
    printf("\n");
}
