#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int i;
} info;

typedef struct {
    info *vetFila;
    int tamVetor;
    int frente; /* indexa o início da Fila */
    int cauda; /*indexa o final da Fila */
    int tamanhoDaFila; /*num de elementos*/
} Fila;

Fila * cria(int tamVet, int tam);
Fila * destroi(Fila *p);
int buscaNaFrente(Fila *p, info *reg);
int buscaNaCauda(Fila *p, info *reg);
int testaVazia(Fila *p);
int testaCheia(Fila *p);
int reinicializa(Fila *p);
int enfileira(Fila *p, info *novo);
int desenfileira(Fila *p, info *reg); 

Fila *cria(int tamVet, int tam){

    Fila *f = NULL;
    f = malloc(sizeof(Fila));
    if(f){
        f->tamVetor = tamVet;
        f->cauda = -1;
        f->frente = 0;
        f->tamanhoDaFila = 0;
        f->vetFila = malloc(tamVet * tam);    
    }else{
        free(f);
        f = NULL;
    }

    return f;
}

Fila *destroi(Fila *f){
    free(f->vetFila);
    free(f);
    return NULL;
}

int enfileira(Fila *f, info *novo){

    if(testaCheia(f)){
        info descarte;
        desenfileira(f, &descarte);
    }

    f->cauda = (f->cauda + 1) % f->tamVetor;

    f->vetFila[f->cauda] = *novo;
    f->tamanhoDaFila++;

    return 1;
}

int desenfileira(Fila *f, info *reg){

    if(testaVazia(f)) return 0;

    *reg = f->vetFila[f->frente];
    f->frente = (f->frente+1) % f->tamVetor;
    f->tamanhoDaFila--;

    return 1;
}

int buscaNaFrente(Fila *f, info *reg){

    if(testaVazia(f)) return 0;
    *reg = f->vetFila[f->frente];
    return 1;
}

int buscaNaCauda(Fila *f, info *reg){

    if(testaVazia(f)) return 0;
    *reg = f->vetFila[f->cauda];
    return 1;
}

int testaVazia(Fila *f){
    return (f->tamanhoDaFila == 0);
}

int testaCheia(Fila *f){
    return (f->tamanhoDaFila == f->tamVetor);
}

int reinicializa(Fila *f){

    f->cauda = -1;
    f->frente = 0;
    f->tamanhoDaFila = 0;

    return 1;
}

void mostraFila(Fila *f) {
    for (int i = 0; i < f->tamanhoDaFila; i++) {
        int pos = (f->frente + i) % f->tamVetor;
        printf(" %d", f->vetFila[pos].i);
    }
}

int main(int argc, char *argv[]){
    int qtd = atoi(argv[1]);
    Fila *f = cria(qtd, sizeof(info));

    int n;
    while(1){
        scanf("%d", &n);
        if(n == -1) break;
        
        if(n == -2){
            if(testaVazia(f)){
                printf("Fila vazia");
            }
            else{
                printf("Fila:");
                mostraFila(f);
                printf("\n");
            }
        }
        else{
            info reg;
            reg.i = n;
            enfileira(f, &reg);
        }
    
    }
}