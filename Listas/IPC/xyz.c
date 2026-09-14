#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>

int n = 1;
int passo = 1;
pthread_mutex_t mtx1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_z = PTHREAD_COND_INITIALIZER, cond_y = PTHREAD_COND_INITIALIZER;


void *x(void *arg){
    pthread_mutex_lock(&mtx1);
    n = n * 16;
    passo = 2;
    pthread_cond_signal(&cond_z);
    pthread_mutex_unlock(&mtx1);
    return NULL;
}

void *z(void *arg){
    pthread_mutex_lock(&mtx1);
    while(passo < 2){
        pthread_cond_wait(&cond_z, &mtx1);
    }

    n = n + 40;
    passo = 3;
    pthread_cond_signal(&cond_y);
    pthread_mutex_unlock(&mtx1);
    return NULL;
}

void *y(void *arg){
    pthread_mutex_lock(&mtx1);
    while(passo < 3){
        pthread_cond_wait(&cond_y, &mtx1);
    }    
    n /= 7;
    pthread_mutex_unlock(&mtx1);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thr[3];
    int rc;
    

    rc = pthread_create(&thr[0], NULL, x, NULL);
    assert(rc == 0);
    
    
    rc = pthread_create(&thr[1], NULL, y, NULL);
    assert(rc == 0);
    rc = pthread_create(&thr[2], NULL, z, NULL);
    assert(rc == 0);
    
    for(int i = 0;i < 1;i++){
        rc = pthread_join(thr[i], NULL);
        assert(rc == 0);
    }

    printf("Valor Final de N: %d\n", n);
    return 0;
}
