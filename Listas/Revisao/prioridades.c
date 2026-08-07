#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct info {
    char palavra[32];
    int prioridade;
} Info;

typedef struct noFSE {
    Info dados;
    struct noFSE *atras;
} No;

typedef struct fse {
    No *frente;
    No *cauda;
    int quantidade;
} FSE;

FSE* cria() {
    FSE *f = (FSE *) malloc(sizeof(FSE));
    if (f != NULL) {
        f->frente = NULL;
        f->cauda = NULL;
        f->quantidade = 0;
    }
    return f;
}

int vazia(FSE *f) {
    return (f->frente == NULL);
}

int insere(FSE *f, Info valor) {
    No *novo = (No *) malloc(sizeof(No));
    if (novo == NULL) return 0;

    novo->dados = valor;
    novo->atras = NULL;

    if (vazia(f)) f->frente = novo;
    else f->cauda->atras = novo;

    f->cauda = novo;
    f->quantidade++;
    return 1;
}

int remove_(FSE *f, Info *saida) {
    if (vazia(f)) return 0;

    No *aux = f->frente;
    *saida = aux->dados;

    f->frente = aux->atras;
    if (f->frente == NULL) f->cauda = NULL;

    free(aux);
    f->quantidade--;
    return 1;
}

int buscaNaFrente(FSE *f, Info *saida) {
    if (vazia(f)) return 0;
    *saida = f->frente->dados;
    return 1;
}

int tamanho(FSE *f) {
    return f->quantidade;
}

void reinicia(FSE *f) {
    Info lixo;
    while (!vazia(f)) remove_(f, &lixo);
}

void destroi(FSE *f) {
    if (f != NULL) {
        reinicia(f);
        free(f);
    }
}


typedef struct noLista {
    int prioridade;
    FSE *fila;
    struct noLista *prox;
} NoLista;

typedef struct lista {
    NoLista *inicio;  
} Lista;

Lista* criaLista() {
    Lista *l = (Lista *) malloc(sizeof(Lista));
    if (l != NULL) l->inicio = NULL;
    return l;
}

FSE* obtemFila(Lista *l, int n) {
    NoLista *ant = NULL;
    NoLista *aux = l->inicio;

    while (aux != NULL && aux->prioridade < n) {
        ant = aux;
        aux = aux->prox;
    }

    if (aux != NULL && aux->prioridade == n) return aux->fila;

    NoLista *novo = (NoLista *) malloc(sizeof(NoLista));
    if (novo == NULL) return NULL;

    novo->prioridade = n;
    novo->fila = cria();
    novo->prox = aux;

    if (ant == NULL) l->inicio = novo;
    else ant->prox = novo;

    return novo->fila;
}

FSE* buscaFila(Lista *l, int n) {
    NoLista *aux = l->inicio;
    while (aux != NULL && aux->prioridade < n) aux = aux->prox;
    if (aux != NULL && aux->prioridade == n) return aux->fila;
    return NULL;
}

void destroiLista(Lista *l) {
    NoLista *aux = l->inicio;
    while (aux != NULL) {
        NoLista *tmp = aux;
        aux = aux->prox;
        destroi(tmp->fila);
        free(tmp);
    }
    free(l);
}


void mostraFila(FSE *f, int n) {
    printf("Prioridade %d:", n);
    No *aux = f->frente;
    while (aux != NULL) {
        printf(" %s", aux->dados.palavra);
        aux = aux->atras;
    }
    printf("\n");
}

void mostrarIgual(Lista *l, int n) {
    FSE *f = buscaFila(l, n);
    if (f != NULL) mostraFila(f, n);
}

void mostrarMenores(Lista *l, int n) {
    NoLista *aux = l->inicio;
    while (aux != NULL && aux->prioridade <= n) {
        mostraFila(aux->fila, aux->prioridade);
        aux = aux->prox;
    }
}


int main(int argc, char *argv[]) {
    Lista *l = criaLista();
    if (l == NULL) return 1;

    char s[32];
    int n;

    while (scanf("%31s", s) == 1) {
        if (strcmp(s, "FIM") == 0) break;
        if (scanf("%d", &n) != 1) break;

        if (strcmp(s, "IGUAL") == 0) {
            mostrarIgual(l, n);
        }
        else if (strcmp(s, "MENIG") == 0) {
            mostrarMenores(l, n);
        }
        else {
            FSE *f = obtemFila(l, n);
            if (f != NULL) {
                Info reg;
                strcpy(reg.palavra, s);
                reg.prioridade = n;
                insere(f, reg);
            }
        }
    }

    destroiLista(l);
    return 0;
}