#include <stdio.h>
#include <stdlib.h>

struct Node{
    int x;
    int y;
    struct Node *prox;
};

typedef struct Node node;

int tam;

void startStack(node *PILHA);
int emptyStack(node *PILHA);
void freeNode(node *PILHA);
void push(node *PILHA, int x, int y);
node *pop(node *PILHA);
void shuffle(node *PILHA);
void swap(node *no1, node *no2);

void startStack(node *PILHA){
    PILHA->prox = NULL;
    tam = 0;
}

int emptyStack(node *PILHA){
    if(PILHA->prox == NULL)
        return 1;
    else
        return 0;
}

node *aloca(){
    node *novo = (node *) malloc(sizeof(node));
    if(!novo){
        printf("Sem memória disponivel!\n");
        exit(1);
    } else {
        return novo;
    }
}

void freeNode(node *PILHA){
    if(!emptyStack(PILHA)){
        node *proxNode,
        *atual;

        atual = PILHA->prox;
        while(atual != NULL){
            proxNode = atual->prox;
            free(atual);
            atual = proxNode;
        }
    }
}

void push(node *PILHA, int x, int y){
    node *novo=aloca();
    novo->x = x;
    novo->y = y;

    novo->prox = NULL;

    if(emptyStack(PILHA))
        PILHA->prox = novo;
    else{
        node *tmp = PILHA->prox;

        while(tmp->prox != NULL)
            tmp = tmp->prox;

        tmp->prox = novo;
    }
    tam++;
}


node *pop(node *PILHA){
    if(PILHA->prox == NULL){
        printf("PILHA ja vazia\n\n");
        return NULL;
    }else{
        node *ultimo = PILHA->prox,
        *penultimo = PILHA;

        while(ultimo->prox != NULL){
            penultimo = ultimo;
            ultimo = ultimo->prox;
        }

        penultimo->prox = NULL;
        tam--;
        return ultimo;
    }
}

void exibe(node *PILHA){
    if(emptyStack(PILHA)){
        printf("PILHA vazia!\n\n");
        return ;
    }

    node *tmp;
    tmp = PILHA->prox;
    printf("PILHA: ");
    while(tmp != NULL){
        printf("%d %d\t", tmp->x, tmp->y);
        tmp = tmp->prox;
    }
    printf("\n\n");
}

void shuffle(node *PILHA){
    int i, j;
    node *tmp, *aux;

    srand(time(NULL));
    for (i = 1; i < tam; i++) {
        int index = rand() % tam;

        tmp = PILHA->prox;
        for (j = 1; j < i; j++) {
            tmp = tmp->prox;
        }

        printf("%d\n", index);
        aux = PILHA->prox;
        for (j = 1; j < index; j++) {
            aux = aux->prox;
        }

        swap(tmp, aux);
    }
}

void swap(node *no1, node *no2){
    node *tmp = no1;
    no1->prox = no2->prox;
    no2->prox = tmp->prox;
}
