#ifndef STACK_H
#define STACK_H

struct Node{
    int x;
    int y;
    struct Node *prox;
};

typedef struct Node node;

int tam;

void startStack(node *PILHA);
void show(node *PILHA);
void freeStack(node *PILHA);
void push(node *PILHA, int x, int y);
node *pop(node *PILHA);

#endif
