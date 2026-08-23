#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

#define NELEM 2000000

struct parametros{
    int *v;
    long tp;
};

void *conta(void *arg){
    long j, total = 0;
    struct parametros *p = (struct parametros *) arg;
    for(j = 0; j < NELEM; j++) {
	  if(p->tp == 0 && (p->v[j] % 2) == 0)
	        total++;
	  else if(p->tp == 1 && ((p->v[j] % 2) != 0))
	        total++;
    }

    pthread_exit((void*) total);
}

int main(int argc, char *argv[]){

    int v[NELEM], rc, j;
    struct timeval tv_ini, tv_fim;
    unsigned long time_diff, sec_diff, usec_diff, msec_diff;

    struct parametros pt[2];
    pt[0].v = v;
    pt[0].tp = 0;

    pt[1].v = v;
    pt[1].tp = 1;


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
        rc = pthread_create(&num[j], NULL, conta, (void *) &pt[j]);
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
    
    pthread_exit(NULL);

}

