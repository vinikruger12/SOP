#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

// Implementação de barreira compatível com macOS (Darwin)
#ifdef __APPLE__
typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int trip_count;
} pthread_barrier_t;

int pthread_barrier_init(pthread_barrier_t *barrier, const void *attr, unsigned int count) {
    if(count == 0) return -1;
    pthread_mutex_init(&barrier->mutex, 0);
    pthread_cond_init(&barrier->cond, 0);
    barrier->trip_count = count;
    barrier->count = 0;
    return 0;
}

int pthread_barrier_wait(pthread_barrier_t *barrier) {
    pthread_mutex_lock(&barrier->mutex);
    ++(barrier->count);
    if(barrier->count >= barrier->trip_count) {
        barrier->count = 0;
        pthread_cond_broadcast(&barrier->cond);
        pthread_mutex_unlock(&barrier->mutex);
        return 1;
    } else {
        pthread_cond_wait(&barrier->cond, &barrier->mutex);
        pthread_mutex_unlock(&barrier->mutex);
        return 0;
    }
}
#endif

#define N 8 // Defina N como um valor divisível pelo número de threads T

unsigned int matriz[N][N];
unsigned int *local_mins;
int T;
pthread_barrier_t barreira;

void *encontrar_minimo(void *arg) {
    long id = (long) arg;
    int linhas_por_thread = N / T;
    int inicio = id * linhas_por_thread;
    int fim = inicio + linhas_por_thread;

    unsigned int min_local = UINT_MAX;

    // (1) Cada thread encontra o menor elemento em sua partição (linhas atribuídas)
    for (int i = inicio; i < fim; i++) {
        for (int j = 0; j < N; j++) {
            if (matriz[i][j] < min_local) {
                min_local = matriz[i][j];
            }
        }
    }

    local_mins[id] = min_local;

    // (4) Espera na barreira até que todas as threads tenham encontrado seus mínimos
    pthread_barrier_wait(&barreira);

    // (2) Conta quantas threads encontraram um elemento estritamente menor que o seu
    int menores_count = 0;
    for (int i = 0; i < T; i++) {
        if (local_mins[i] < min_local) {
            menores_count++;
        }
    }

    // (3) Relata o resultado da thread atual
    printf("Thread %ld: menor elemento=%u, threads com elementos menores=%d\n", id, min_local, menores_count);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <numero_de_threads>\n", argv[0]);
        exit(-1);
    }

    T = atoi(argv[1]);
    if (N % T != 0) {
        printf("Erro: N (%d) deve ser divisível por T (%d)\n", N, T);
        exit(-1);
    }

    local_mins = malloc(T * sizeof(unsigned int));
    pthread_barrier_init(&barreira, NULL, T);

    // Preenche a matriz com valores aleatórios
    srand(42); 
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matriz[i][j] = rand() % 100;
        }
    }

    pthread_t threads[T];
    for (long i = 0; i < T; i++) {
        pthread_create(&threads[i], NULL, encontrar_minimo, (void*) i);
    }

    for (int i = 0; i < T; i++) {
        pthread_join(threads[i], NULL);
    }

    // O programa principal (main) reporta o menor elemento absoluto da matriz
    unsigned int menor_global = UINT_MAX;
    for (int i = 0; i < T; i++) {
        if (local_mins[i] < menor_global) {
            menor_global = local_mins[i];
        }
    }

    printf("Menor elemento da matriz: %u\n", menor_global);

    free(local_mins);
    return 0;
}