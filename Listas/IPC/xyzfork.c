#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

struct dados{
    int valor;
    sem_t cond_z;
    sem_t cond_y;
};


void *x(void *arg){
    struct dados *ptr = (struct dados *)arg;
    ptr->valor = ptr->valor * 16;
    sem_post(&ptr->cond_z);
    return NULL;
}

void *z(void *arg){
    struct dados *ptr = (struct dados *)arg;
    sem_wait(&ptr->cond_z);
    
    ptr->valor = ptr->valor + 40;
    sem_post(&ptr->cond_y);
    return NULL;
}

void *y(void *arg){
    struct dados *ptr = (struct dados *)arg;
    sem_wait(&ptr->cond_y);    
    ptr->valor /= 7;
    
    return NULL;
}

int main(int argc, char *argv[]) {

    struct dados *ptr;
    int fd;

    fd = shm_open("/exemplo", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    ftruncate(fd, sizeof(struct dados));

    ptr = mmap(NULL, sizeof(struct dados),PROT_READ | PROT_WRITE, MAP_SHARED, fd,0);
    ptr->valor = 1;
    sem_init(&ptr->cond_z, 1, 0);
    sem_init(&ptr->cond_y, 1, 0);
    
    if(fork() == 0){
        y(ptr);
        exit(0);
    }


    if(fork() == 0){
        x(ptr);
        exit(0);
    }


    if (fork() == 0) {
        z(ptr);
        exit(0);
    }

    wait(NULL);
    wait(NULL);
    wait(NULL);

    printf("Valor Final de N: %d\n", ptr->valor);
    return 0;
}
