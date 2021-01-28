#include <iostream>
#include "ListaAdjacencia.h"
#include "Fila.h"
#include "Pilha.h"

using namespace std;

static int lbl[maxV]; //label (rotulo) - marca os vertices como -1 se nao visitado e 0 como visitado ou quando foi visitado na busca em profundidade //para busca em largura: se v eh o k-esimo vertice visitado entao lbl[v] = k-1
static int parnt[maxV]; //vetor de pais - parnt[w] eh o pai de w

int cnt; //contador

static int time; //outro contador
static int d[maxV]; //registra o momento em que o vertice eh descoberto
static int f[maxV]; //registra o momento em que o vertice eh finalizado

static int cc[maxV]; //n de componentes

static int color[maxV];//pintar os grafos pra determinar se ha biparticao

static int bcnt = 0;

static int pre[maxV], low[maxV]; //encontrar as pontes

static int sc[maxV]; //numero do componente que o vertice sc[v] pertence (se o vertice v pertence ao k-esimo componente entao sc[v] == k-1)
static Vertex sop[maxV], sopR[maxV]; //retorna qual componente o vertice i pertence
static int id;

static int dist[maxV]; // Distancia de um vertice t em relacao a s, se dist[t] == g->V, entao nao existe caminho de s a t

No* alocarNo(Vertex w, No* prox){
    No* p = (No*) malloc(sizeof(No));
    p->w = w;
    p->prox = prox;
    return p;
}

/*--------------------------------------------------------
------------------LISTA DE ADJACENCIA--------------------
--------------------------------------------------------*/

//Devolve o endereco de um novo digrafo com vertices de 0,..., V-1 e nenhum arco
Digrafo* alocarDigrafo(int V){
    Vertex v;
    Digrafo* g = (Digrafo*) malloc(sizeof(Digrafo));
    g->V = V;
    g->A = 0;
    g->adj = (No**) malloc(sizeof(No*) * V);
    for(v = 0; v < V; v++){
        g->adj[v] = NULL;
    }
    return g;
}

//Insere um arco v-w no digrafo g
void inserirArco(Digrafo* g, Vertex v, Vertex w){
    //INSERCAO ORDENADA NA LISTA
    No* p, *ant = NULL;
    if(v == w) return; //nao aloca laço

    p = g->adj[v];
    while(p){
        if(p->w == w) //evita arco paralelo
            return;
        if(p->w > w)
            break;
        ant = p;
        p = p->prox;
    }
    No* atual = alocarNo(w,NULL);
    if(ant){
        atual->prox = ant->prox;
        ant->prox = atual;    
    }else{
        g->adj[v] = atual;
    }
     
    g->A++;
}
//Insercao nao eh tao eficiente pois quando o grafo cresce o consumo de tempo da insercao tambem cresce [O(V^2)]

//Insere um arco v-w no digrafo g (transfere a verificacao para o usuario)
void inserirDigrafoA(Digrafo* g, Vertex v, Vertex w){
    g->adj[v] = alocarNo(w, g->adj[v]);
    g->A++;
}

//Imprime o digrafo g
void exibirDigrafo(Digrafo* g){
    Vertex v;
    No* p;

    for(v = 0; v < g->V; v++){
        printf("%2d:",v);
        for(p = g->adj[v]; p != NULL; p = p->prox){
            printf("%2d",p->w);
        }
        printf("\n");
    }
} //Theta(V+A)

//Insere uma aresta v-w no digrafo g
void inserirAresta(Digrafo* g, Vertex v, Vertex w){
    inserirArco(g,v,w);
    inserirArco(g,w,v);
}

//Remove um arco v-w do digrafo g
void removerArco(Digrafo* g, Vertex v, Vertex w){
    No* p, *ant = NULL, *aux;

    p = g->adj[v];

    while(p){
        if(p->w == w){
            aux = p;
            if(!ant){
                g->adj[v] = p->prox;
            }else{
                ant->prox = p->prox;
            }
            free(aux);
            break;
        }
        ant = p;
        p = p->prox;
    }
    g->A--;
}

//Remove um arco v-w do digrafo g
void removerAresta(Digrafo* g, Vertex v, Vertex w){
    removerArco(g,v,w);
    removerArco(g,w,v);
}

/*--------------------------------------------------------
--------------------CAMINHOS EM DIGRAFOS------------------
--------------------------------------------------------*/

//(Funcao auxiliar)Percorre o digrafo g rotulando os vertices e definindo os pais dos vertices (parnt[])
void caminho(Digrafo* g, Vertex v){
    No* p;
    lbl[v] = 0;
    for(p = g->adj[v]; p != NULL; p = p->prox){
        if(lbl[p->w] == -1){
            parnt[p->w] = v;
            caminho(g,p->w);
        }
    }
}//O(V + A)

//Recebe um digrafo g e retorna true se existe caminho entre s e t ou false caso contrario
bool caminhoDigrafo(Digrafo* g, Vertex s, Vertex t){
    Vertex v;
    for(v = 0; v < g->V; v++){
        lbl[v] = -1;
        parnt[v] = -1;
    }

    parnt[s] = s; //pai de s eh ele mesmo
    caminho(g,s);
    if(lbl[t] == -1) return false;
    return true;
} //O(V+A)

/*--------------------------------------------------------
--------------------BUSCA EM PROFUNDIDADE-----------------
--------------------------------------------------------*/
//Depth-First Search (DFS)

//Examina sistematicamente todos os vertices e todos os arcos de um digrafo, tal que cada arco eh examinado somente uma vez
void buscaEmProfundidadeDigrafo(Digrafo* g){
    Vertex v;
    cnt = 0;
    for(v = 0; v < g->V; v++)
        lbl[v] = -1;

    for(v = 0; v < g->V; v++)
        for(v = 0; v < g->V; v++){
            if(lbl[v] == -1)
                buscaEmProfundidade(g, v);
        }
}//Theta(V+A) MatrizAdjacencia: Theta(V^2)

//(Funcao auxiliar) Visita todos os vertices do digrafo que podem ser atingidos a partir de v, rotulando-os
void buscaEmProfundidade(Digrafo* g, Vertex v){
    No* p;
    lbl[v] = cnt++;
    for(p = g->adj[v]; p != NULL; p = p->prox){
        if(lbl[p->w] == -1)
            buscaEmProfundidade(g,p->w);
    }
} 

//----------------ARBORESCENCIA DE BUSCA EM PROFUNDIDADE----------------

//----------------CLASSIFICAÇÃO DOS ARCOS DE ARBORESCENCIA--------------
//d[v] --> registra o momento em que v foi descoberto (visitado pela primeira vez)
//f[v] --> registra o momento em que v foi completamente examinado

//(Funcao auxiliar) Visita todos os vertices do digrafo que podem ser atingidos a partir de v, registrando o momento em que um vertice foi descoberto e finalizado
void buscaEmProfundidade2(Digrafo* g, Vertex v){
    No* p;
    d[v] = time++;
    
    for(p = g->adj[v]; p != NULL; p = p->prox){
        if(d[p->w] == -1){
            parnt[p->w] = v;
            buscaEmProfundidade2(g,p->w);
        }
    }
    if(parnt[v] == -1) //raiz de uma arborescencia que compoem a floresta da busca em profundidade(DFS forest)
        parnt[v] = v;
    f[v] = time++;
}

//Visita dos os vertices e arcos do digrafo g, realizando uma busca em profundidade
void buscaEmProfundidadeDigrafo2(Digrafo* g){
    Vertex v;
    time = 0;
    for(v = 0; v < g->V; v++){
        d[v] = -1;
        f[v] = -1;
        parnt[v] = -1;
    }
    for(v = 0; v < g->V; v++){
        if(d[v] == -1){
            buscaEmProfundidade2(g,v);
        }
    }
}

//Dado um arco de um digrafo, printa sua classificacao na DFS forest
void classificaArco(Digrafo* g, Vertex v, Vertex w){
    buscaEmProfundidadeDigrafo2(g);
    cout << v << "-" << w << ": ";
    if(d[v] < d[w] && d[w] < f[w] && f[w] < f[v]){
        if(parnt[w] == v)
            cout << "arco de arborescencia" << endl;
        else
            cout << "arco descendente" << endl;
    } 
    if(d[w] < d[v] && d[v] < f[v] && f[v] < f[w])
        cout << "arco de retorno" << endl;
    
    if(d[w] < f[w] && f[w] < d[v] && d[v] < f[v])
        cout << "arco cruzado" << endl;
}

//v-w eh arco de arborescencia ou descendente se e somente se: d[v] < d[w] < f[w] < f[v]
//v-w eh arco de retorno se e somente se: d[w] < d[v] < f[v] < f[w]
//v-w eh arco cruzado se e somente se: d[w] < f[w] < d[v] < f[v]

//---------------------------------CICLOS---------------------------------

//Verifica se o digrafo possui um ciclo
bool DigrafoCiclo1(Digrafo* g){
    Vertex v;
    No* p;
    bool result;
    for(v = 0; v < g->V; v++){
        for(p=g->adj[v]; p != NULL; p = p->prox){
            result = caminhoDigrafo(g,p->w,v);
            if(result)
                return true;
        }
    }
    return false;
}
//lista de adjacência: O(A(V+A))
//matriz de adjacência: O(AV^2)

//Em relação a qualquer floresta de busca em profundidade: todo arco de retorno pertence a um ciclo e todo ciclo tem um arco de retorno

//Verifica se o digrafo possui um ciclo
bool DigrafoCiclo2(Digrafo* g){
    Vertex v;
    time = 0;
    for(v = 0; v < g->V;v++){
        d[v] = -1;
        f[v] = -1;
        parnt[v] = -1;
    }

    for(v = 0; v < g->V;v++){
        if(d[v] == -1){
            parnt[v] = v;
            if(ciclo2(g,v))
                return true;
        }
    }
    return false;
}//O(V + A)

//(Funcao auxiliar) Dado um vertice v do digrafo v, verifica se existe um arco de retorno
bool ciclo2(Digrafo* g, Vertex v){
    No* p;
    Vertex w;
    d[v] = time++;

    for(p = g->adj[v]; p != NULL; p = p->prox){
        w = p->w;
        if(d[w] == -1){
            parnt[w] = v;
            if(ciclo2(g,w))
                return true;
        } else if(f[w] == -1)
            return true;
    }
    f[v] = time++;
    return false;
}
//lista de adjacencia: O(V+A)
//matriz de adjacencia: O(V^2)

//Verifica se o arco v-w junto com a floresta DFS formam um ciclo
bool cicloCertificadoExistencia(Digrafo* g, Vertex v, Vertex w){
    DigrafoCiclo2(g);
    Vertex t;
    No* p = g->adj[v];
    while(p){
        if(p->w == w)
            //if(!st_caminho(g, w, v))
                //return false;
        if(p->w > w)
            return false;
        
        p = p->prox;
    }
    return true;
}

//Certificado de existencia do caminho de s a t
bool st_caminho(Digrafo* g, Vertex s, Vertex t){
    Vertex v, w;
    No* p;

    if(parnt[t] == -1 || parnt[s] == -1)
        return false;
    
    for(w = t; w != s; w = v){
        v = parnt[w];
        p = g->adj[v];
        while(p){
            if(p->w == w)
                break;
            if(p->w > w)
                return false;
            
            p = p->prox;
        }
    }
    return true;
} //se tiver ciclo o programa entre em loop infinito

/* 
    Uma forma de certificar que existe o ciclo: retornar a sequência dos ciclos
    Outra forma: construir a arborescencia + o arco de retorno
*/

//--------------------------------ORDENACAO TOPOLOGICA-------------------------------------
/*
    Algoritmo de eliminação de fontes: armazena em ts[0..i-1] uma permutação de um subconjunto do conjunto de vertices de g e devolve o valor de i
    se i = g->v entao ts[0..i-1] eh uma ordenacao topologica de g
    caso contrario, g nao eh um DAG

    ts[] = topological sorting (ordenação topológica)
 */
//Algoritmo de eliminação de fontes: se i = g->v, então ts[] é uma ordenação topológica de g. Caso contrário, g não é um dag (possui ciclos).  | OBS: Caso seja um DAG, a funcao tem a ordenacao topologica do digrafo
bool VerificarDigrafoAciclico(Digrafo* g, Vertex ts[]){
    int i;
    int in[maxV]; //numero de entradas no vertice
    Vertex v;
    No* p;

    for(v = 0; v < g->V; v++){
        in[v] = 0;
    }

    for(v = 0; v < g->V; v++){
        for(p = g->adj[v]; p != NULL; p = p->prox){
            in[p->w]++;
        }
    }
 
    Fila f;
    inicializarFila(&f);
    for(v=0; v < g->V; v++){
        if(in[v] == 0)
            entrarFila(&f, v);
    }
    for(i = 0; !filaVazia(&f); i++){
        v = sairFila(&f);
        ts[i] = v;
        for(p=g->adj[v]; p != NULL; p = p->prox){
            if(--in[p->w] == 0)
                entrarFila(&f,p->w);
        }
    }
    cout << "Ordenacao topologica: ";
    for(int j = 0; j < g->V; j++){
        cout << ts[j] << " ";
    } cout << endl;

    if(i == g->V)
        return true;
    return false;
}
//O(V+A)
//se tiver ciclo entra em loop (testei e nao entrou em loop)

//Recebe um DAG g e armazena em ts[0..V-1] uma ordenacao topologica de g
void  OrdenacaoTopologicaDigrafo(Digrafo* g, Vertex* ts){
    Vertex v;
    cnt = g->V-1;
    for(v = 0; v < g->V; v++){
        lbl[v] = -1;
        ts[v] = -1;
    }
    for(v = 0; v < g->V; v++){
        if(lbl[v] == -1)
            TS_BuscaEmProfundidade(g,v,ts);
    }

    cout << "Ordenacao topologica: ";
    for(int j = 0; j < g->V; j++){
        cout << ts[j] << " ";
    } cout << endl;
} //O(V + A)

//(Funcao auxiliar) Realiza uma busca em profundidade preenchendo a ordenacao topologica de g em ts
void TS_BuscaEmProfundidade(Digrafo* g, Vertex v, Vertex* ts){
    No* p;
    lbl[v] = 0;
    for(p = g->adj[v]; p != NULL; p = p->prox){
        if(lbl[p->w] == -1){
            TS_BuscaEmProfundidade(g,p->w,ts);
        }
    }
    ts[cnt--] = v;
}
//O(V+A)

//Verifica se um vetor ts[] armazena uma ordenacao topologica dos vertices de um grafo g
bool certificadoInexistenciaTS(Digrafo* g, Vertex* ts, int* idx){
    //idx indice do vertice na ordenacao topologica
    No* p;
    Vertex v;
    for(v = 0; v < g->V; v++){
        idx[ts[v]] = v;
    }
    for(v=0; v < g->V;v++){
        for(p = g->adj[v]; p != NULL; p = p->prox){
            if(idx[v] > idx[p->w])
                return false;
        }
    }
    return true;
}

//Dado um digrafo g e um vetor ts[], verificar se existe um ciclo (se nao houver, ts[] eh uma ordenacao topologia de g)
bool DigrafoCicloTS(Digrafo* g,Vertex* ts){
    Vertex v;
    time = 0;
    cnt = g->V-1;

    for(v = 0; v < g->V; v++){
        parnt[v] = -1;
        f[v] = -1;
        d[v] = -1;
    }

    for(v = 0; v < g->V; v++){
        if(d[v] == -1){
            parnt[v] = v;
            if(cicloTS(g,v,ts))
                return true;
        }
    }
    return false;
}

//(Funcao auxiliar) Dado um vertice v verifica todos os vertices alcancados por ele, verificando se existe ciclo
bool cicloTS(Digrafo* g, Vertex v,Vertex* ts){
    No* p;
    d[v] = time++;
    for(p = g->adj[v]; p != NULL; p = p->prox){
        if(d[p->w] == -1){
            parnt[p->w] = v;
            if(cicloTS(g,p->w,ts))
                return true;
        } else if(f[p->w] == -1) //arco de retorno
            return true;
    }
    f[v] = time++;
    ts[cnt--] = v;
    return false;
}
//lista de adjacencia: O(V+A)
//matriz de adjacencia: O(V^2)

/*---------------------------------------------------------
-------CICLOS E COMPONENTES EM GRAFOS NAO-ORIENTADOS-------
----------------------------------------------------------*/

//Problema: decidir se dado um grafo g possui um ciclo
bool grafoCiclo1(Digrafo* g){
    Vertex v,w;
    No* p;
    bool output;
    for(v = 0; v < g->V; v++){
        for(p = g->adj[v]; p != NULL; p = p->prox){
            w = p->w;
            if(v < w){
                //removeArcoGrafo(g,w,v);
                output = caminhoDigrafo(g,w,v);
                //inserirArcoGrafo(g,w,v);
                if(output)
                    return true;
            }
        }
    }
    return false;
}
// lista de adjacencias: O(A(V+A))
// matriz de adjacencia: O(V^2)

//Buscando melhorar a eficiencia do codigo, temos um grafoCiclo2 que se baseia em:
/*
    Em relacao a qualquer floresta DFS:
    todo arco de retorno que nao eh anti-paralelo a um arco da arborescencia pertence a um ciclo nao-trivial
    todo ciclo nao trivial tem arco de retorno que nao eh anti-paralelo a um arco da arborescencia
*/

bool grafoCiclo2(Digrafo* g){
    Vertex v;
    for(v = 0; v < g->V; v++){
        lbl[v] = -1;
    }

    for(v = 0; v < g->V; v++){
        if(lbl[v] == -1){
            parnt[v] = v;
            if (cicloGrafo(g,v)){
                return true;
            }  
        }
    }
    return false;
}

bool cicloGrafo(Digrafo* g, Vertex v){
    No* p;
    lbl[v] = 1;

    for(p = g->adj[v]; p != NULL; p = p->prox){
        Vertex w = p->w;
        if(lbl[w] == -1){
            parnt[w] = v;
            if(cicloGrafo(g,w)){
                return true;
            }else if(parnt[w] != v){
                return true;
            }
        } 
    }
    return false;
}
// lista de adjacencia: O(V+A)
// matriz de adjacencia: O(V^2)

//Problema: calcular o numero de componentes

int componentesGrafo(Digrafo* g){
    Vertex v;
    int id = 0;
    for(v = 0; v < g->V; v++){
        cc[v] = -1;
    }

    for(v = 0; v < g->V; v++){
        if(cc[v] == -1){
            buscaEmProfundidadeCC(g, v, id++);
        }
    }
    return id;
}
// O(V+A)
void buscaEmProfundidadeCC(Digrafo* g, Vertex v, int id){
    No* p;
    cc[v] = id;
    for(p = g->adj[v]; p != NULL; p = p->prox){
        if(cc[p->w] == -1){
            buscaEmProfundidadeCC(g,p->w, id);
        }
    }
}


//A funcao devolve true se o grafo eh bipartido e falso caso contrario
bool GRAPHtwoColor(Digrafo* g){
    Vertex v;
    for(v = 0; v < g->V; v++){
        color[v] = -1;
    }

    for(v = 0; v < g->V; v++){
        if(color[v] == -1){
            if(dfsRclr(g,v,0) == 0){
                return false;
            }
        }
    }
    return true;
}

int dfsRclr(Digrafo* g, Vertex v, int c){
    No* p;
    
    color[v] = 1-c;

    for(p=g->adj[v]; p != NULL; p = p->prox){
        Vertex w = p->w;
        if(color[w] == -1){
            if(dfsRclr(g,w,1-c) == 0){
                return 0;
            }else if(color[w] == 1-c){
                return 0;
            }
        }
    }
    return 1;
}
//CONSUMO DE TEMPO: O(V +A) lista de ajacencia e O(V^2) p/ matriz

/*-------------------------------------------------------------
-----------------------PROCURANDO PONTES-----------------------
-------------------------------------------------------------*/

//Algoritmo 1: printa todas as pontes

void allBridges1(Digrafo* g){
    Vertex v, w;
    No* p;
    bool output;

    for(v = 0; v < g->V; v++){
        for(p = g->adj[v]; p != NULL; p = p->prox){
            w = p->w;
            if(v < w){
                //removerArco(g,w,v);
                output = caminhoDigrafo(g,w,v);
                if(!output){
                    bcnt++;
                    cout << v << "-" << w << endl;
                }
            }
        }
    }
}
//LISTA DE ADJACENCIAS: O(A(V+A))
//MATRIZ DE ADJACENCIAS: O(AV^2)

//PONTES E BUSCA EM PROFUNDIDADE
/* Em uma floresta DFS, um dos dois arcos de cada ponte será um arco da arborescencia
*/

void allBridges(Digrafo* g){
    Vertex v;
    cnt = 0;
    bcnt = 0;

    for(v = 0; v < g->V; v++){
        pre[v] = -1;
    }

    for(v = 0; v < g->V; v++){
        if(pre[v] == -1){
            parnt[v] = v;
            bridgeR(g,v);
        }
    }

}

void bridgeR(Digrafo* g, Vertex v){ //ponte
    No* p;
    Vertex w;
    pre[v] = cnt++;
    low[v] = pre[v];

    for(p = g->adj[v]; p != NULL; p = p->prox){
        if(pre[w = p->w] == -1){
            parnt[w] = v;
            bridgeR(g,w);

            if(low[v] > low[w]){
                low[v] = low[w];
            }
            if(low[w] == pre[w]){
                bcnt++;
                printf("%d-%d\n",v,w);
            }
        } else if(w != parnt[v] && low[v] > pre[w]){
            low[v] = pre[v];
        }
    }
}
//O(V+A)

//-----CONSTRUIR COMPONENTES FORTEMENTE CONEXOS DE UM DIGRAFO---
//algoritmo de força bruta
//strongly connected
bool strongreach(Digrafo* g, Vertex s, Vertex t){
    return sc[s] == sc[t];
}

int DIGRAPHsc1(Digrafo* g){
    Vertex v = 0, w = 0;
    int n;

    Digrafo* grafoAuxiliar = alocarDigrafo(g->V);

    for(v = 0; v < g->V; v++){
        for(w = v + 1; w < g->V; w++){
            if(caminhoDigrafo(g,v,w) && caminhoDigrafo(g,w,v)){
                inserirArco(grafoAuxiliar, v,w);
            }
        }
    }
    n = componentesGrafo(grafoAuxiliar);

    for(v = 0; v < g->V; v++){
        sc[v] = cc[v];
    }
    return n;
}

//CUSTO PARA LISTA DE ADJACENCIA: O(V^2(V + A))
//CUSTO PARA MATRIZ DE ADJACENCIA: O(V^4)

/*--------------------------------------------------
---------------ALGORITMO DE KOSARAJU--------------------------
--------------------------------------------------*/
// a funcao devolve o numero de componentes fortementes conexos de um digrafo g
int DIGRAPHsc(Digrafo* g){
    Vertex v;
    int id, i;
    Digrafo* R = DigrafoReverso(g);
    cnt = 0;

    for(v = 0; v < R->V; v++){
        sc[v] = -1;
    }

    for(v = 0; v < R->V; v++){
        if(sc[v] == -1){
            dfsRsc(R, v, 0);
        }
    }

    for(i = 0; i < g->V; i++){
        sopR[i] = sop[i];
    }
    id = 0;
    cnt = id;

    for(v = 0; v < g->V; v++){
        sc[v] = -1;
    }

    for(i = g->V-1; i > 0; i--){
        dfsRsc(g, sopR[i], id++);
    }

   // DIGRAPHdestroy(R);
    return id;
}

void dfsRsc(Digrafo* g, Vertex v, int id){
    No* p;
    sc[v] = id;

    for(p = g->adj[v]; p != NULL; p = p->prox){
        if(sc[p->w] == -1){
            dfsRsc(g, p->w, id);
        }
    }
    sop[cnt++] = v;

}

Digrafo* DigrafoReverso(Digrafo* g){
    Vertex v;
    No* p;

    Digrafo* R = alocarDigrafo(g->V);

    for(v = 0; v < g->V; v++){
        for(p = g->adj[v]; p != NULL; p = p->prox){
            inserirArco(R,p->w, v); //no slide ta g se der errado olhar aqui
        }
    }
    return R;
}
//O(V + A)

//----------------------------BUSCA EM LARGURA---------------------------

//Visita todos os vertices do digrafo g que podem ser alcancados a partir de s, descrevendo a arborescencia com raiz s(fazer isso em outra funcao)
void buscaEmLarguraDigrafo(Digrafo* g, Vertex s){
    Vertex v;
    cnt = 0;
    No* p;
    for(v = 0; v < g->V; v++)
        lbl[v] = -1;
    
    //inicializarFila(g->V);
    lbl[s] = cnt++;
    //QUEUEput(s);
    /*
    while(!QUEUEempty()){
        v = QUEUEget();
        p = g->adj[v];
        while(p){
            if(lbl[p->w] == -1){
                lbl[p->w] = cnt++;
                QUEUEput(p->w);
            }
            p = p->prox;
        }
    }
    QUEUEfree();
    */
}//O(V+A) //MATRIZ: O(V^2)

//--------------------ARBORESCENCIA DA BFS---------------------
//busca em largura usando parnt

//---------------------CAMINHOS MINIMOS-------------------

//Dado um digrafo g e um vertice s, determinar a distancia de s aos demais vertices do digrafo
void distanciaDigrafo(Digrafo* g, Vertex s){
    Vertex v, w;
    No* p;

    for(v = 0; v < g->V; v++){
        dist[v] = g->V;
        parnt[v] = -1;
    }

    Fila* f;
    inicializarFila(f);
    dist[s] = 0;
    parnt[s] = s;
    //QUEUEput(s);
    /*
    while(!QUEUEempty()){
        v = QUEUEget();
        p = g->adj[v];
        while(p){
            if(dist[p->w] == g->V){
                dist[p->w] = dist[v]+1;
                parnt[w] = v;
                QUEUEput(p->w);
            }
        } 
    }
    QUEUEfree();
    */
}//O(V+A) //matriz de adjacencia:  O(V^2)

//dado um digrafo g e um vertice s, printa o caminho minimo de s a x
void printarArborescencia(Digrafo* g, Vertex s, Vertex x){
    Vertex v;
    for(v = x; v != s; v = parnt[v]){
        //bota numa pilha
    }
    //imprime a pilha

}