#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "huffman.h"

// Declaração das funções auxiliares do codigo.c
U8 pega_bit_codigo(Codigo* c, U64 pos);
boolean escreve_codigo(FILE* out, Codigo* cod);

// ==================== FILA DE PRIORIDADE ====================

FilaPrioridade* cria_fila(int capacidade) {
    FilaPrioridade* fp = (FilaPrioridade*)malloc(sizeof(FilaPrioridade));
    fp->dados = (No**)malloc(capacidade * sizeof(No*));
    fp->tamanho = 0;
    fp->capacidade = capacidade;
    return fp;
}

void destroi_fila(FilaPrioridade* fp) {
    free(fp->dados);
    free(fp);
}

static void sobe_heap(FilaPrioridade* fp, int idx) {
    while (idx > 0) {
        int pai = (idx - 1) / 2;
        if (fp->dados[pai]->frequencia <= fp->dados[idx]->frequencia)
            break;
        No* temp = fp->dados[pai];
        fp->dados[pai] = fp->dados[idx];
        fp->dados[idx] = temp;
        idx = pai;
    }
}

static void desce_heap(FilaPrioridade* fp, int idx) {
    while (1) {
        int esq = 2 * idx + 1;
        int dir = 2 * idx + 2;
        int menor = idx;
        if (esq < fp->tamanho && fp->dados[esq]->frequencia < fp->dados[menor]->frequencia)
            menor = esq;
        if (dir < fp->tamanho && fp->dados[dir]->frequencia < fp->dados[menor]->frequencia)
            menor = dir;
        if (menor == idx) break;
        No* temp = fp->dados[idx];
        fp->dados[idx] = fp->dados[menor];
        fp->dados[menor] = temp;
        idx = menor;
    }
}

boolean insere_fila(FilaPrioridade* fp, No* no) {
    if (fp->tamanho == fp->capacidade) return false;
    fp->dados[fp->tamanho] = no;
    sobe_heap(fp, fp->tamanho);
    fp->tamanho++;
    return true;
}

No* extrai_min(FilaPrioridade* fp) {
    if (fp->tamanho == 0) return NULL;
    No* min = fp->dados[0];
    fp->dados[0] = fp->dados[fp->tamanho - 1];
    fp->tamanho--;
    if (fp->tamanho > 0) desce_heap(fp, 0);
    return min;
}

int tamanho_fila(FilaPrioridade* fp) {
    return fp->tamanho;
}

// ==================== CONSTRUÇÃO DA ÁRVORE ====================

No* constroi_arvore_huffman(U64 frequencias[256]) {
    int count = 0;
    for (int i = 0; i < 256; i++)
        if (frequencias[i] > 0) count++;
    
    if (count == 0) return NULL;
    
    FilaPrioridade* fp = cria_fila(count);
    
    for (int i = 0; i < 256; i++) {
        if (frequencias[i] > 0) {
            No* no = (No*)malloc(sizeof(No));
            no->simbolo = (U8)i;
            no->frequencia = frequencias[i];
            no->esq = no->dir = NULL;
            insere_fila(fp, no);
        }
    }
    
    while (tamanho_fila(fp) > 1) {
        No* a = extrai_min(fp);
        No* b = extrai_min(fp);
        No* novo = (No*)malloc(sizeof(No));
        novo->simbolo = 0;
        novo->frequencia = a->frequencia + b->frequencia;
        novo->esq = a;
        novo->dir = b;
        insere_fila(fp, novo);
    }
    
    No* raiz = extrai_min(fp);
    destroi_fila(fp);
    return raiz;
}

// ==================== GERAÇÃO DOS CÓDIGOS ====================

void gera_codigos(No* raiz, Codigo* codigos[256], Codigo* codigo_atual) {
    if (raiz->esq == NULL && raiz->dir == NULL) {
        if (codigos[raiz->simbolo] == NULL) {
            codigos[raiz->simbolo] = (Codigo*)malloc(sizeof(Codigo));
            novo_codigo(codigos[raiz->simbolo]);
        }
        clone(*codigo_atual, codigos[raiz->simbolo]);
        return;
    }
    
    if (raiz->esq) {
        adiciona_bit(codigo_atual, 0);
        gera_codigos(raiz->esq, codigos, codigo_atual);
        joga_fora_bit(codigo_atual);
    }
    
    if (raiz->dir) {
        adiciona_bit(codigo_atual, 1);
        gera_codigos(raiz->dir, codigos, codigo_atual);
        joga_fora_bit(codigo_atual);
    }
}

// ==================== LIBERAÇÃO DA ÁRVORE ====================

void libera_arvore(No* raiz) {
    if (raiz == NULL) return;
    libera_arvore(raiz->esq);
    libera_arvore(raiz->dir);
    free(raiz);
}

// ==================== SERIALIZAÇÃO DA ÁRVORE ====================

void serializa_arvore(No* raiz, Codigo* bits) {
    if (raiz->esq == NULL && raiz->dir == NULL) {
        // Folha: bit 1 + símbolo (8 bits)
        adiciona_bit(bits, 1);
        for (int i = 7; i >= 0; i--) {
            U8 bit = (raiz->simbolo >> i) & 1;
            adiciona_bit(bits, bit);
        }
    } else {
        // Nó interno: bit 0
        adiciona_bit(bits, 0);
        serializa_arvore(raiz->esq, bits);
        serializa_arvore(raiz->dir, bits);
    }
}

No* desserializa_arvore(Codigo* bits, U64* pos) {
    U8 bit = pega_bit_codigo(bits, *pos);
    (*pos)++;
    
    if (bit == 1) {
        // Folha: lê 8 bits do símbolo
        U8 simbolo = 0;
        for (int i = 0; i < 8; i++) {
            U8 b = pega_bit_codigo(bits, *pos);
            (*pos)++;
            simbolo = (simbolo << 1) | b;
        }
        No* no = (No*)malloc(sizeof(No));
        if (!no) return NULL;
        no->simbolo = simbolo;
        no->frequencia = 0;
        no->esq = no->dir = NULL;
        return no;
    } else {
        // Nó interno
        No* no = (No*)malloc(sizeof(No));
        if (!no) return NULL;
        no->simbolo = 0;
        no->frequencia = 0;
        no->esq = desserializa_arvore(bits, pos);
        no->dir = desserializa_arvore(bits, pos);
        return no;
    }
}

// ==================== LEITOR DE BITS ====================

typedef struct {
    FILE* file;
    U8 buffer;
    int bits_restantes;
} LeitorBits;

static void inicia_leitor(LeitorBits* lb, FILE* f) {
    lb->file = f;
    lb->bits_restantes = 0;
    lb->buffer = 0;
}

static int proximo_bit(LeitorBits* lb) {
    if (lb->bits_restantes == 0) {
        size_t lidos = fread(&lb->buffer, 1, 1, lb->file);
        if (lidos == 0) return -1;
        lb->bits_restantes = 8;
    }
    lb->bits_restantes--;
    int bit = (lb->buffer >> lb->bits_restantes) & 1;
    return bit;
}

// ==================== COMPACTAÇÃO ====================

boolean compacta_arquivo(const char* entrada, const char* saida) {
    FILE* in = fopen(entrada, "rb");
    if (!in) return false;
    
    // 1. Contagem de frequências
    U64 frequencias[256] = {0};
    U8 byte;
    U64 total_bytes = 0;
    while (fread(&byte, 1, 1, in) == 1) {
        frequencias[byte]++;
        total_bytes++;
    }
    rewind(in);
    
    // 2. Construção da árvore
    No* raiz = constroi_arvore_huffman(frequencias);
    if (!raiz) {
        fclose(in);
        return false;
    }
    
    // 3. Geração dos códigos
    Codigo* codigos[256] = {NULL};
    Codigo cod_atual;
    if (!novo_codigo(&cod_atual)) {
        libera_arvore(raiz);
        fclose(in);
        return false;
    }
    gera_codigos(raiz, codigos, &cod_atual);
    free_codigo(&cod_atual);
    
    // 4. Serialização da árvore
    Codigo cabecalho_bits;
    if (!novo_codigo(&cabecalho_bits)) {
        for (int i = 0; i < 256; i++)
            if (codigos[i]) { free_codigo(codigos[i]); free(codigos[i]); }
        libera_arvore(raiz);
        fclose(in);
        return false;
    }
    serializa_arvore(raiz, &cabecalho_bits);
    U64 tamanho_cabecalho = cabecalho_bits.tamanho;
    
    // 5. Abertura do arquivo de saída
    FILE* out = fopen(saida, "wb");
    if (!out) {
        free_codigo(&cabecalho_bits);
        for (int i = 0; i < 256; i++)
            if (codigos[i]) { free_codigo(codigos[i]); free(codigos[i]); }
        libera_arvore(raiz);
        fclose(in);
        return false;
    }
    
    // Escreve cabeçalho: total_bytes (8 bytes) + tamanho_cabecalho (8 bytes)
    fwrite(&total_bytes, sizeof(U64), 1, out);
    fwrite(&tamanho_cabecalho, sizeof(U64), 1, out);
    
    // Escreve os bits da árvore serializada
    U64 bytes_cabecalho = (tamanho_cabecalho + 7) / 8;
    fwrite(cabecalho_bits.byte, 1, bytes_cabecalho, out);
    
    // 6. Compressão dos dados
    Codigo dados_comprimidos;
    if (!novo_codigo(&dados_comprimidos)) {
        fclose(in); fclose(out);
        free_codigo(&cabecalho_bits);
        for (int i = 0; i < 256; i++)
            if (codigos[i]) { free_codigo(codigos[i]); free(codigos[i]); }
        libera_arvore(raiz);
        return false;
    }
    
    rewind(in);
    while (fread(&byte, 1, 1, in) == 1) {
        Codigo* cod = codigos[byte];
        for (U64 i = 0; i < cod->tamanho; i++) {
            U8 bit = pega_bit_codigo(cod, i);
            adiciona_bit(&dados_comprimidos, bit);
        }
    }
    
    // Escreve os dados comprimidos
    U64 bytes_dados = (dados_comprimidos.tamanho + 7) / 8;
    fwrite(dados_comprimidos.byte, 1, bytes_dados, out);
    
    // Limpeza
    fclose(in);
    fclose(out);
    free_codigo(&cabecalho_bits);
    free_codigo(&dados_comprimidos);
    for (int i = 0; i < 256; i++)
        if (codigos[i]) { free_codigo(codigos[i]); free(codigos[i]); }
    libera_arvore(raiz);
    
    return true;
}

// ==================== DESCOMPACTAÇÃO ====================

boolean descompacta_arquivo(const char* entrada, const char* saida) {
    FILE* in = fopen(entrada, "rb");
    if (!in) return false;
    
    // Lê cabeçalho
    U64 total_bytes, tamanho_cabecalho;
    if (fread(&total_bytes, sizeof(U64), 1, in) != 1 ||
        fread(&tamanho_cabecalho, sizeof(U64), 1, in) != 1) {
        fclose(in);
        return false;
    }
    
    // Lê os bytes do cabeçalho serializado
    U64 bytes_cabecalho = (tamanho_cabecalho + 7) / 8;
    U8* buffer_cabecalho = (U8*)malloc(bytes_cabecalho);
    if (!buffer_cabecalho) {
        fclose(in);
        return false;
    }
    if (fread(buffer_cabecalho, 1, bytes_cabecalho, in) != bytes_cabecalho) {
        free(buffer_cabecalho);
        fclose(in);
        return false;
    }
    
    // Prepara Codigo para desserialização
    Codigo bits_cabecalho;
    bits_cabecalho.byte = buffer_cabecalho;
    bits_cabecalho.capacidade = bytes_cabecalho * 8;
    bits_cabecalho.tamanho = tamanho_cabecalho;
    
    // Desserializa a árvore
    U64 pos = 0;
    No* raiz = desserializa_arvore(&bits_cabecalho, &pos);
    if (!raiz) {
        free(buffer_cabecalho);
        fclose(in);
        return false;
    }
    
    // Abre arquivo de saída
    FILE* out = fopen(saida, "wb");
    if (!out) {
        free(buffer_cabecalho);
        libera_arvore(raiz);
        fclose(in);
        return false;
    }
    
    // Decodifica os dados comprimidos
    LeitorBits lb;
    inicia_leitor(&lb, in);
    
    U64 bytes_emitidos = 0;
    No* atual = raiz;
    while (bytes_emitidos < total_bytes) {
        int bit = proximo_bit(&lb);
        if (bit < 0) break;
        
        atual = (bit == 0) ? atual->esq : atual->dir;
        
        if (atual->esq == NULL && atual->dir == NULL) {
            fwrite(&atual->simbolo, 1, 1, out);
            bytes_emitidos++;
            atual = raiz;
        }
    }
    
    fclose(in);
    fclose(out);
    free(buffer_cabecalho);
    libera_arvore(raiz);
    
    return (bytes_emitidos == total_bytes);
}