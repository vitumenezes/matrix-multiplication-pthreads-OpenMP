#ifndef AUX_STACK_H
#define AUX_STACK_H

typedef struct {
    int x;
    int y;
    struct node *prox;
} node;

int tam;

int menu(void);
void inicia(node *PILHA);
void opcao(node *PILHA, int op);
void exibe(node *PILHA);
void libera(node *PILHA);
void push(node *PILHA);
node *pop(node *PILHA);
