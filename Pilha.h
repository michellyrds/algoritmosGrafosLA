typedef struct p{
    int chave;
    struct p* prox;
} pNo; //No da pilha

typedef struct{
    pNo* topo;
} Pilha;

void inicializarPilha(Pilha* pi);
int pop(Pilha* pi);
void push(Pilha* pi, int ch);
bool pilhaVazia(Pilha* pi);
