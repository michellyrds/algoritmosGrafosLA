#include <iostream>
#include "Pilha.h"
using namespace std;

void inicializarPilha(Pilha* pi){
    pi->topo = NULL;
}

int pop(Pilha* pi){
    int resp = -1;
    if(pi->topo){
        resp = pi->topo->chave;
        pNo* aux = pi->topo;
        pi->topo = pi->topo->prox;
        free(aux);
    }
    return resp;
}

void push(Pilha* pi, int ch){
    pNo* novo = (pNo*) malloc(sizeof(pNo));
    novo->chave = ch;
    novo->prox = pi->topo;
    pi->topo = novo;
}

bool pilhaVazia(Pilha* pi){
    if(!pi->topo)
        return true;
    return false;
}