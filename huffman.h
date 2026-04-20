#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "meustiposonlinegdb.h"
#include "codigo.h"
#include <stdio.h>

// Criando o nó da árvore de Huffman
typedef struct No {
    U8 simbolo;
    U64 frequencia;
    struct No* esq;
    struct No* dir;
} No;

// Fila de prioridade (min-heap)
typedef struct {
    No** dados;
    int tamanho;
    int capacidade;
} FilaPrioridade;

// Funções da fila de prioridade
FilaPrioridade* cria_fila(int capacidade);
void destroi_fila(FilaPrioridade* fp);
boolean insere_fila(FilaPrioridade* fp, No* no);
No* extrai_min(FilaPrioridade* fp);
int tamanho_fila(FilaPrioridade* fp);

// Funções do Huffman
No* constroi_arvore_huffman(U64 frequencias[256]);
void gera_codigos(No* raiz, Codigo* codigos[256], Codigo* codigo_atual);
void libera_arvore(No* raiz);

// Serialização da árvore (para cabeçalho)
void serializa_arvore(No* raiz, Codigo* bits);
No* desserializa_arvore(Codigo* bits, U64* pos);

// Compactação e descompactação
boolean compacta_arquivo(const char* entrada, const char* saida);
boolean descompacta_arquivo(const char* entrada, const char* saida);

#endif