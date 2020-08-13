typedef struct s{
    int chave;
    struct s* prox;
} fNo; //No da fila

typedef struct{
    fNo* inicio;
    fNo* fim;
} Fila;

void inicializarFila(Fila* f);
void entrarFila(Fila* f, int ch);
int sairFila(Fila* f);
bool filaVazia(Fila* f);