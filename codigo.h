#ifndef CODIGO
#define CODIGO
#include "meustiposonlinegdb.h"

typedef struct
{
    U8* byte;          // vetor dinâmico de bytes
    U64 capacidade;    // em bits
    U64 tamanho;       // em bits
} Codigo;

boolean novo_codigo(Codigo* c);
void free_codigo(Codigo* c);
boolean adiciona_bit(Codigo* c, U8 valor);
boolean joga_fora_bit(Codigo* c);
boolean pega_byte(Codigo c, U8 qual, U8* b);
char* toString(Codigo c);
boolean clone(Codigo original, Codigo* copia);

#endif