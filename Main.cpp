#include <iostream>
#include "ListaAdjacencia.h"
#include "Fila.h"
#include "Pilha.h"

using namespace std;

void alocarDigrafo1(Digrafo* g);
void alocarDigrafo2(Digrafo* g);
void alocarDigrafo3(Digrafo* g);

int main(){

    Digrafo* digrafo;
    digrafo = alocarDigrafo(6); //Aloca um grafo com 10 vertices de 0...9
    alocarDigrafo1(digrafo);
    //exibirDigrafo(digrafo);
    int ts[digrafo->V], idx[digrafo->V];
    OrdenacaoTopologicaDigrafo(digrafo, ts);
    if(certificadoInexistenciaTS(digrafo, ts, idx))
        cout << "ta serto" << endl;
    
    return 0;
}

void alocarDigrafo1(Digrafo* g){ //DAG
    inserirArco(g,0,2);
    inserirArco(g,0,3);
    inserirArco(g,0,4);
    inserirArco(g,2,1);
    inserirArco(g,2,4);
    inserirArco(g,3,4);
    inserirArco(g,3,5);
    inserirArco(g,4,1);
    inserirArco(g,4,5);
    inserirArco(g,5,1);
}

void alocarDigrafo2(Digrafo* g){
    inserirArco(g,0,5);
    inserirArco(g,0,7);
    inserirArco(g,1,5);
    inserirArco(g,2,1);
    inserirArco(g,3,4);
    inserirArco(g,3,6);
    inserirArco(g,4,0);
    inserirArco(g,4,7);
    inserirArco(g,5,2);
    inserirArco(g,5,7);
    inserirArco(g,6,3);
    inserirArco(g,6,4);
    inserirArco(g,7,1);
    /*
    classificaArco(digrafo,0,5);
    classificaArco(digrafo,0,7);
    classificaArco(digrafo,1,5);
    classificaArco(digrafo,2,1);
    classificaArco(digrafo,3,4);
    classificaArco(digrafo,3,6);
    classificaArco(digrafo,4,0);
    classificaArco(digrafo,4,7);
    classificaArco(digrafo,5,2);
    classificaArco(digrafo,5,7);
    classificaArco(digrafo,6,3);
    classificaArco(digrafo,6,4);
    classificaArco(digrafo,7,1);
    */
}

void alocarDigrafo3(Digrafo* g){ //digrafo com ciclo
    inserirArco(g,0,2);
    inserirArco(g,0,3);
    inserirArco(g,0,4);
    inserirArco(g,1,5);
    inserirArco(g,2,1);
    inserirArco(g,3,4);
    inserirArco(g,4,1);
    inserirArco(g,4,2);
    inserirArco(g,4,5);
    inserirArco(g,5,3);
}