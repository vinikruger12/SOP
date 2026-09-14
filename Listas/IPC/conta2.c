#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>

pthread_barrier_t barreira;

void *conta(void *arg){
    pthread_barrier_wait(&barreira);
    long total = 0;
    for(long i = 0;i < (long) arg;i++){
        total++;
    }

    pthread_exit((void*) total);
}

int main(int argc, char *argv[]){

    struct timeval tv_ini, tv_fim;
    unsigned long time_diff, usec_diff, msec_diff;

    long alvo = 1L << 31;
    long n = atoi(argv[1]);
    int rc;
    long port = alvo/n;
    long resto = alvo % n;
    pthread_t t[n];
    int nthr = n;
    pthread_barrier_init(&barreira, NULL, nthr);
    void *status;

    gettimeofday(&tv_ini, NULL);

    for(long i = 0;i < n;i++){
        
        rc = pthread_create(&t[i], NULL, conta, (void*) port);
        if(rc){
            printf("ERRO - rc=%d\n", rc);
            exit(-1);
        }
    }

    long total = 0;
    for(long i = 0;i < n;i++){
        pthread_join(t[i], &status);
        total += (long) status;
    }

    for(long i = 0;i < resto;i++){
        total++;
    }

    gettimeofday(&tv_fim, NULL);

    time_diff = (1000000L * tv_fim.tv_sec + tv_fim.tv_usec) - (1000000L * tv_ini.tv_sec + tv_ini.tv_usec);

    msec_diff = time_diff / 1000;
    usec_diff = time_diff % 1000000L;

    printf("total: %ld\n", total);
    printf("Tempo de execucao: %lu.%03lu mseg\n",
    msec_diff, usec_diff % 1000);

    pthread_exit(NULL);

}