#include <iostream>
#include "Fila.h"

using namespace std;

void inicializarFila(Fila* f){
    f->inicio = NULL;
    f->fim = NULL;
}

void entrarFila(Fila* f, int ch){
    fNo* novo = (fNo*) malloc(sizeof(fNo));
    novo->chave = ch;
    novo->prox = NULL;

    if(f->fim){
        f->fim->prox = novo;
    }else{
        f->inicio = novo;
    }
    f->fim = novo;
}

int sairFila(Fila* f){
    int resp = -1;
    if(f->inicio){
        resp = f->inicio->chave;
        fNo* aux = f->inicio;
        f->inicio = f->inicio->prox;
        free(aux);
        if(!f->inicio)
            f->fim = NULL;
    }
    return resp;
}

bool filaVazia(Fila* f){
    if(!f->inicio)
        return true;
    return false;
}