#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 5

void *PrintHello(void *arg) {
    long tid = (long)arg;
    printf("Alo da thread %ld\n", tid);
    pthread_exit(NULL);
}

void *quadrado(void *arg){
    printf("n: %ld , n^n: %ld \n", (long)arg, (long)arg * (long) arg);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    pthread_t numeros[NUM_THREADS + 3];
    int rc;
    long t;

    for(t=0; t<NUM_THREADS; t++){
        printf("main: criando thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);

        if(rc){
            printf("ERRO - rc=%d\n", rc);
            exit(-1);
        }
    }

    for(long i = 0;i < NUM_THREADS + 3;i++){
        printf("main: criando thread %ld\n", i + t);
        rc = pthread_create(&numeros[i], NULL, quadrado, (void *) i);

        if(rc){
            printf("ERRO - rc=%d\n", rc);
            exit(-1);
        }

    }


    pthread_exit(NULL);
}