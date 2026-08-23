#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 5

void *PrintHello(void *arg) {
    long tid = (long)arg;
    printf("Alo da thread %ld\n", tid);

    printf("n: %ld , n^n: %ld \n", tid, tid * tid);

    pthread_exit((void *)(tid*tid));
}


int main (int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    void *status;
    long somatorio = 0;

    for(t=0; t<NUM_THREADS; t++){
        printf("main: criando thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);

        if(rc){
            printf("ERRO - rc=%d\n", rc);
            exit(-1);
        }

    }

    t = 0;
    for(t = 0;t < NUM_THREADS;t++){
        pthread_join(threads[t], &status);
        somatorio += (long) status;
    }

    
    printf("somatorio: %ld\n", somatorio);


    pthread_exit(NULL);
}