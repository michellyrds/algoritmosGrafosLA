#define maxV 20
#define Vertex int

typedef struct No{
    Vertex w;
    No* prox;
} No;

typedef struct Digrafo{
    int V; //Numero de vertices
    int A; //Numero de arcos
    No** adj; //Lista de adjacencia
} Digrafo;


No* alocarNo(Vertex w, No* prox);
Digrafo* alocarDigrafo(int V);
void inserirArco(Digrafo* g, Vertex v, Vertex w);
void inserirDigrafoA(Digrafo* g, Vertex v, Vertex w);
void exibirDigrafo(Digrafo* g);
void inserirAresta(Digrafo* g, Vertex v, Vertex w);
void removerArco(Digrafo* g, Vertex v, Vertex w);
void removerAresta(Digrafo* g, Vertex v, Vertex w);

void caminho(Digrafo* g, Vertex v);
bool caminhoDigrafo(Digrafo* g, Vertex s, Vertex t);

void buscaEmProfundidadeDigrafo(Digrafo* g);
void buscaEmProfundidade(Digrafo* g, Vertex v);
void buscaEmProfundidade2(Digrafo* g, Vertex v);
void buscaEmProfundidadeDigrafo2(Digrafo* g);
void classificaArco(Digrafo* g, Vertex v, Vertex w);

bool DigrafoCiclo1(Digrafo* g);
bool DigrafoCiclo2(Digrafo* g);
bool ciclo2(Digrafo* g, Vertex v);
bool cicloCertificadoExistencia(Digrafo* g, Vertex v, Vertex w);
bool st_caminho(Digrafo* g, Vertex s, Vertex t);

bool VerificarDigrafoAciclico(Digrafo* g, Vertex ts[]); //Verifica se o grafo eh um DAG ou nao
void OrdenacaoTopologicaDigrafo(Digrafo* g, Vertex* ts);
void TS_BuscaEmProfundidade(Digrafo* g, Vertex v, Vertex* ts);

bool certificadoInexistenciaTS(Digrafo* g,Vertex* ts, int* idx);
bool DigrafoCicloTS(Digrafo* g,Vertex* ts);
bool cicloTS(Digrafo* g, Vertex v,Vertex* ts);

bool grafoCiclo1(Digrafo* g);
bool grafoCiclo2(Digrafo* g);
bool cicloGrafo(Digrafo* g, Vertex v);
int componentesGrafo(Digrafo* g);
void buscaEmProfundidadeCC(Digrafo* g, Vertex v, int id);
bool GRAPHtwoColor(Digrafo* g);
int dfsRclr(Digrafo* g, Vertex v, int c);
void allBridges1(Digrafo* g);
void allBridges(Digrafo* g);
void bridgeR(Digrafo* g, Vertex v);
bool strongreach(Digrafo* g, Vertex s, Vertex t);
int DIGRAPHsc1(Digrafo* g);
int DIGRAPHsc(Digrafo* g);
void dfsRsc(Digrafo* g, Vertex v, int id);
Digrafo* DigrafoReverso(Digrafo* g);
