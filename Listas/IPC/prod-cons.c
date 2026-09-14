#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define TRUE 1
#define N 5

sem_t mutex; /* exclusão mútua no acesso ao buffer */
sem_t empty; /* conta lugares vazios no buffer */
sem_t full; /* conta lugares preenchidos no buffer */

int buffer[N];
int in = 0, out = 0;

void *produtor(void *arg) {
    int item = 1;
    while(TRUE){
        sem_wait(&empty);
        sem_wait(&mutex);
        
        buffer[in] = item;
        in = (in + 1) % N;
        printf("Produtor inseriu: %d\n", item);
        sem_post(&mutex);
        sem_post(&full); 
        item++;
        sleep(1);
    }
}

void *consumidor(void *arg) {
    int item;
    while(TRUE){
        sem_wait(&full);
        sem_wait(&mutex);
        item = buffer[out];
        out = (out + 1) % N;
        printf("Consumidor consumiu: %d", item);
        sem_post(&mutex);
        sem_post(&empty);
        sleep(2);
    }
}

int main(){
    pthread_t prod, cons;
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&prod, NULL, produtor, NULL);
    pthread_create(&cons, NULL, consumidor, NULL);

    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}