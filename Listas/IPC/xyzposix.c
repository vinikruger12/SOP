#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>
#include <semaphore.h>
int n = 1;
int passo = 1;

sem_t cond_z, cond_y;


void *x(void *arg){
    n = n * 16;
    passo = 2;
    sem_post(&cond_z);
    return NULL;
}

void *z(void *arg){
    
    sem_wait(&cond_z);
    
    n = n + 40;
    sem_post(&cond_y);
    return NULL;
}

void *y(void *arg){
    sem_wait(&cond_y);    
    n /= 7;
    
    return NULL;
}

int main(int argc, char *argv[]) {
    sem_init(&cond_y, 0, 0);
    sem_init(&cond_z, 0, 0);
    pthread_t thr[3];
    int rc;
    

    rc = pthread_create(&thr[0], NULL, x, NULL);
    assert(rc == 0);
    
    
    rc = pthread_create(&thr[1], NULL, y, NULL);
    assert(rc == 0);
    rc = pthread_create(&thr[2], NULL, z, NULL);
    assert(rc == 0);
    
    for(int i = 0;i < 3;i++){
        rc = pthread_join(thr[i], NULL);
        assert(rc == 0);
    }

    printf("Valor Final de N: %d\n", n);
    return 0;
}
