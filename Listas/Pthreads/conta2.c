#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>

void *conta(void *arg){
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

/*
Tempo de acordo com N

n = 1
total: 2147483648
Tempo de execucao: 5645.839 mseg

n = 2
total: 2147483648
Tempo de execucao: 3319.690 mseg

n = 3
total: 2147483648
Tempo de execucao: 2737.713 mseg

n = 4
total: 2147483648
Tempo de execucao: 2568.221 mseg

n = 5
total: 2147483648
Tempo de execucao: 2486.524 mseg

n = 6
total: 2147483648
Tempo de execucao: 2831.476 mseg

n = 7
total: 2147483648
Tempo de execucao: 2827.205 mseg

n = 8
total: 2147483648
Tempo de execucao: 2416.786 mseg
*/