#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// ========== SOLUÇÃO PARA MACOS ==========
// O macOS (Darwin) não possui pthread_barrier_t nativo. 
// Este bloco implementa a barreira usando mutex e cond para rodar no seu Mac.
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
        barrier->count = 0; // Reseta para a próxima rodada
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
// ========================================

int *fila;
int pos_fila = 0;
int N;

pthread_barrier_t barreira;
pthread_mutex_t mutex_fila; // Mutex para proteger a inserção na fila

void *thread_tarefa(void *arg) {
    long id = (long) arg;

    // Repete 10 vezes
    for(int rodada = 1; rodada <= 10; rodada++) {
        
        // (1) Incrementa uma variável 10^9 vezes
        // Uso de 'volatile' obriga o GCC a não otimizar e ignorar o for.
        volatile long total = 0;
        for(long i = 0; i < 1000000000L; i++) {
            total++;
        }

        // (2) Insere o valor id no final da fila
        pthread_mutex_lock(&mutex_fila);
        int meu_lugar = pos_fila; // Salva a posição em que a thread entrou
        fila[pos_fila] = id;
        pos_fila++;
        pthread_mutex_unlock(&mutex_fila);

        // (3) Se for a última thread a terminar, imprime a fila
        if (meu_lugar == N - 1) {
            printf("Rodada %02d | Ordem de termino: ", rodada);
            for (int i = 0; i < N; i++) {
                printf("%d ", fila[i]);
            }
            printf("\n");
            
            // Reseta o contador da fila para a próxima rodada
            pos_fila = 0; 
        }

        // (4) Espera na barreira até todas terminarem a rodada
        pthread_barrier_wait(&barreira);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <numero_de_threads>\n", argv[0]);
        exit(-1);
    }

    N = atoi(argv[1]);
    pthread_t t[N];
    fila = malloc(N * sizeof(int));

    pthread_barrier_init(&barreira, NULL, N);
    pthread_mutex_init(&mutex_fila, NULL);

    // Cria as threads
    for(long i = 0; i < N; i++) {
        int rc = pthread_create(&t[i], NULL, thread_tarefa, (void*) i);
        if(rc) {
            printf("ERRO - rc=%d\n", rc);
            exit(-1);
        }
    }

    // Aguarda finalização
    for(long i = 0; i < N; i++) {
        pthread_join(t[i], NULL);
    }

    free(fila);
    pthread_mutex_destroy(&mutex_fila);
    // pthread_barrier_destroy(&barreira); // Omitido na versão Apple customizada

    return 0;
}