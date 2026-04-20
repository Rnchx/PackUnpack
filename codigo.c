#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include "codigo.h"

boolean novo_codigo(Codigo* c) {
    c->byte = (U8*)malloc(1 * sizeof(U8));
    if (c->byte == NULL) return false;
    c->byte[0] = 0;
    c->capacidade = 8;
    c->tamanho = 0;
    return true;
}

void free_codigo(Codigo* c) {
    if (c->byte != NULL) free(c->byte);
    c->byte = NULL;
    c->capacidade = 0;
    c->tamanho = 0;
}

boolean adiciona_bit(Codigo* c, U8 valor) {
    // Expande se necessário
    if (c->tamanho == c->capacidade) {
        U64 novos_bytes = (c->capacidade / 8) + 1;
        U8* novo = (U8*)malloc(novos_bytes * sizeof(U8));
        if (novo == NULL) return false;
        
        U64 bytes_atuais = c->capacidade / 8;
        for (U64 i = 0; i < bytes_atuais; i++)
            novo[i] = c->byte[i];
        
        novo[novos_bytes - 1] = 0;
        
        free(c->byte);
        c->byte = novo;
        c->capacidade = novos_bytes * 8;
    }
    
    // Calcula posição do bit (preenche do MSB para o LSB)
    U64 byte_idx = c->tamanho / 8;
    int bit_pos = 7 - (c->tamanho % 8);
    
    if (valor == 1)
        c->byte[byte_idx] |= (1 << bit_pos);
    else
        c->byte[byte_idx] &= ~(1 << bit_pos);
    
    c->tamanho++;
    return true;
}

boolean joga_fora_bit(Codigo* c) {
    if (c->tamanho == 0) return false;
    c->tamanho--;
    return true;
}

boolean pega_byte(Codigo c, U8 qual, U8* b) {
    if (qual >= (c.capacidade / 8)) return false;
    *b = c.byte[qual];
    return true;
}

boolean clone(Codigo original, Codigo* copia) {
    U64 num_bytes = original.capacidade / 8;
    copia->byte = (U8*)malloc(num_bytes * sizeof(U8));
    if (copia->byte == NULL) return false;
    
    memcpy(copia->byte, original.byte, num_bytes);
    copia->capacidade = original.capacidade;
    copia->tamanho = original.tamanho;
    return true;
}

char* toString(Codigo c) {
    if (c.tamanho == 0) {
        char* vazio = (char*)malloc(1);
        if (vazio) vazio[0] = '\0';
        return vazio;
    }
    
    char* retorno = (char*)malloc((c.tamanho + 1) * sizeof(char));
    if (retorno == NULL) return NULL;
    
    for (U64 i = 0; i < c.tamanho; i++) {
        U64 byte_idx = i / 8;
        int bit_pos = 7 - (i % 8);
        retorno[i] = ((c.byte[byte_idx] >> bit_pos) & 1) ? '1' : '0';
    }
    retorno[c.tamanho] = '\0';
    return retorno;
}

// Função auxiliar para ler um bit em determinada posição
U8 pega_bit_codigo(Codigo* c, U64 pos) {
    if (pos >= c->tamanho) return 2;
    U64 byte_idx = pos / 8;
    int bit_pos = 7 - (pos % 8);
    return (c->byte[byte_idx] >> bit_pos) & 1;
}

// Função para escrever código inteiro em arquivo
boolean escreve_codigo(FILE* out, Codigo* cod) {
    U64 bytes_para_escrever = (cod->tamanho + 7) / 8;
    if (bytes_para_escrever == 0) return true;
    size_t escritos = fwrite(cod->byte, 1, bytes_para_escrever, out);
    return escritos == bytes_para_escrever;
}