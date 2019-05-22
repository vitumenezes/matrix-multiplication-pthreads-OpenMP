#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void startStack(node *PILHA){
    PILHA->prox = NULL;
    tam=0;
}

int stackIsEmpty(node *PILHA){
    if(PILHA->prox == NULL)
    return 1;
    else
    return 0;
}

node *aloca(){
    node *novo=(node *) malloc(sizeof(node));
    if(!novo){
        printf("Sem memoria disponivel!\n");
        exit(1);
    } else {
        return novo;
    }
}

void show(node *PILHA){
    if(stackIsEmpty(PILHA)){
        printf("PILHA vazia!\n\n");
        return;
    }

    node *tmp;
    tmp = PILHA->prox;
    printf("PILHA:\t");

    while( tmp != NULL){
        printf("%d%d ", tmp->x, tmp->y);
        tmp = tmp->prox;
    }

    printf("\n");
}

void freeStack(node *PILHA){
    if(!stackIsEmpty(PILHA)){
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
    novo->prox = NULL;
    novo->x = x;
    novo->y = y;

    if(stackIsEmpty(PILHA))
        PILHA->prox=novo;
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
        printf("Pilha ja vazia\n\n");
        return NULL;
    } else {
        node *ultimo = PILHA->prox, *penultimo = PILHA;

        while(ultimo->prox != NULL){
            penultimo = ultimo;
            ultimo = ultimo->prox;
        }

        penultimo->prox = NULL;
        tam--;
        return ultimo;
    }
}
