#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define DEFAULT_N 10

int *v;
int N;

long *ordem;
long prox = 0;

/* protege a fila de threads concluídas */
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *procura(void *arg)
{
    long id = (long)arg;
    long i;
    int valor;

    valor = v[0];

    /* ID par procura maior */
    if ((id % 2) == 0) {
        for (i = 1; i < N; i++) {
            if (v[i] > valor)
                valor = v[i];
        }
    }
    /* ID ímpar procura menor */
    else {
        for (i = 1; i < N; i++) {
            if (v[i] < valor)
                valor = v[i];
        }
    }

    /* insere o ID no fim da fila de concluídas */
    pthread_mutex_lock(&mtx);
    ordem[prox] = id;
    prox++;
    pthread_mutex_unlock(&mtx);

    pthread_exit((void *)(long)valor);
}

int main(int argc, char *argv[])
{
    long nthr, t;
    int i, rc;
    int menor, maior;
    pthread_t *thr;
    void *ret;
    long valor;

    if (argc < 2) {
        fprintf(stderr, "uso: %s <num-threads> [N]\n", argv[0]);
        exit(1);
    }

    nthr = atol(argv[1]);

    if (nthr < 1) {
        fprintf(stderr, "numero de threads invalido\n");
        exit(1);
    }

    if (argc >= 3)
        N = atoi(argv[2]);
    else
        N = DEFAULT_N;

    if (N < 1) {
        fprintf(stderr, "N invalido\n");
        exit(1);
    }

    /* aloca vetor, descritores das threads e fila */
    v = (int *)malloc(N * sizeof(int));
    thr = (pthread_t *)malloc(nthr * sizeof(pthread_t));
    ordem = (long *)malloc(nthr * sizeof(long));

    if ((v == NULL) || (thr == NULL) || (ordem == NULL)) {
        fprintf(stderr, "erro de alocacao\n");
        exit(1);
    }

    /* inicializa vetor */
    srandom(time(NULL));

    for (i = 0; i < N; i++)
        v[i] = (int)random();

    /* calcula menor e maior para validar as threads */
    menor = maior = v[0];

    for (i = 1; i < N; i++) {
        if (v[i] < menor)
            menor = v[i];

        if (v[i] > maior)
            maior = v[i];
    }

    /* cria as threads
       índice 0 -> ID 1
       índice 1 -> ID 2
       ...
    */
    for (t = 0; t < nthr; t++) {
        rc = pthread_create(&thr[t], NULL, procura,
                            (void *)(t + 1));

        if (rc != 0) {
            fprintf(stderr, "erro em pthread_create()\n");
            exit(1);
        }
    }

    /* espera e verifica o resultado de cada thread */
    for (t = 0; t < nthr; t++) {
        rc = pthread_join(thr[t], &ret);

        if (rc != 0) {
            fprintf(stderr, "erro em pthread_join()\n");
            exit(1);
        }

        valor = (long)ret;

        /* ID da thread = t + 1 */
        if (((t + 1) % 2) == 0) {
            if (valor != maior)
                printf("ERRO: thread %ld encontrou %ld, esperado %d\n",
                       t + 1, valor, maior);
        }
        else {
            if (valor != menor)
                printf("ERRO: thread %ld encontrou %ld, esperado %d\n",
                       t + 1, valor, menor);
        }
    }

    /* imprime ordem real de término */
    printf("ordem:");
    for (t = 0; t < nthr; t++)
        printf(" %ld", ordem[t]);
    printf("\n");

    free(v);
    free(thr);
    free(ordem);

    pthread_mutex_destroy(&mtx);

    return 0;
}