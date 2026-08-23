#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>


#define NELEM 2000000
int v[NELEM];

void *conta(void *arg){
    long j, total = 0;
    long tipo = (long) arg;
    for(j = 0; j < NELEM; j++) {
	  if(tipo == 0 && (v[j] % 2) == 0)
	        total++;
	  else if(tipo == 1 && ((v[j] % 2) != 0))
	        total++;
    }

    pthread_exit((void*) total);
}

int main(int argc, char *argv[]){

    int rc, j;
    struct timeval tv_ini, tv_fim;
    unsigned long time_diff, sec_diff, usec_diff, msec_diff;

    srandom(time(NULL));
    for(j = 0; j < NELEM; j++){
	    v[j] = (int)random();
    }

    pthread_t num[2];
    long pares = 0, impares = 0;
    void *status;

    rc = gettimeofday(&tv_ini, NULL);
    if(rc != 0){
	    perror("erro em gettimeofday()");
        exit(1);
    }

    for(long j = 0;j < 2;j++){
        rc = pthread_create(&num[j], NULL, conta, (void *) j);
        if(rc){
            printf("ERRO - rc=%d\n", rc);
            exit(-1);
        }
    }
    
    pthread_join(num[0], &status);
    pares = (long) status;

    pthread_join(num[1], &status);
    impares = (long) status;


    rc = gettimeofday(&tv_fim, NULL);
    if(rc != 0){
	    perror("erro em gettimeofday()");
	    exit(1);
    }

    time_diff = (1000000L*tv_fim.tv_sec + tv_fim.tv_usec) - (1000000L*tv_ini.tv_sec + tv_ini.tv_usec);

    sec_diff = time_diff / 1000000L;
    usec_diff = time_diff % 1000000L;
     
    msec_diff = time_diff / 1000;
     
    printf("O vetor tem %ld numeros pares e %ld numeros impares.\n", pares, impares);
    printf("Tempo de execucao: %lu.%03lu mseg\n", msec_diff, usec_diff%1000);
    
    return 0;
}


/*
VERSOES SEM PTHREADS

1. O vetor tem 1000185 numeros pares e 999815 numeros impares.
Tempo de execucao: 38.086 mseg

2. O vetor tem 999870 numeros pares e 1000130 numeros impares.
Tempo de execucao: 42.806 mseg

3.  vetor tem 1000487 numeros pares e 999513 numeros impares.
Tempo de execucao: 40.889 mseg

4. O vetor tem 1000140 numeros pares e 999860 numeros impares.
Tempo de execucao: 56.609 mseg

5. O vetor tem 999358 numeros pares e 1000642 numeros impares.
Tempo de execucao: 49.070 mseg

*/

/*
VERSOES COM PTHREADS

1. O vetor tem 1000364 numeros pares e 999636 numeros impares.
Tempo de execucao: 22.321 mseg

2. O vetor tem 999744 numeros pares e 1000256 numeros impares.
Tempo de execucao: 26.122 mseg

3.  O vetor tem 999857 numeros pares e 1000143 numeros impares.
Tempo de execucao: 31.235 mseg

4. O vetor tem 1000289 numeros pares e 999711 numeros impares.
Tempo de execucao: 23.384 mseg

5. O vetor tem 999084 numeros pares e 1000916 numeros impares.
Tempo de execucao: 22.124 mseg

*/