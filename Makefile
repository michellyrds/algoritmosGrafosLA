all: Main.cpp ListaAdjacencia.o Fila.o Pilha.o
	g++ -o Grafo Main.cpp ListaAdjacencia.o Fila.o Pilha.o

ListaAdjacencia.o: ListaAdjacencia.cpp ListaAdjacencia.h
	g++ -c ListaAdjacencia.cpp

Fila.o: Fila.cpp Fila.h
	g++ -c Fila.cpp

Pilha.o: Pilha.cpp Pilha.h
	g++ -c Pilha.cpp