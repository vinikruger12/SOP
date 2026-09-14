#include <pthread.h>
pthread_barrier_t bar;

void *thread(void *arg) {

    calcula_parte_1();

    pthread_barrier_wait(&bar);

    calcula_parte_2();

    return NULL;
}

int main() {

    pthread_barrier_init(&bar, NULL, 3);

    /* cria 4 threads */

    ...
}

/*
O valor 3 usado em pthread_barrier_init() está correto?
- Deveria ser 4, já que sao 4 threads.
O que pode acontecer com as 4 threads durante a execução?
- alguma delas pode entrar em conflito.
Qual deveria ser o valor correto?
- 4.
Conceitualmente, o que acontece quando uma thread chama pthread_barrier_wait()?
-sincroniza inicios ou fins de fases.
A barreira pode ser reutilizada em outra rodada depois que todas as threads passarem?
-Pode
*/