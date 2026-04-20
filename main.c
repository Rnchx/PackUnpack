#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codigo.h"
#include "huffman.h"

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Mostra o conteúdo de um arquivo
void mostrar_conteudo(const char* nome) {
    FILE* f = fopen(nome, "r");
    if (f) {
        char buffer[10000];
        size_t len = fread(buffer, 1, sizeof(buffer) - 1, f);
        buffer[len] = '\0';
        printf("   Conteudo: \"%s\"\n", buffer);
        fclose(f);
    }
}

void compactar() {
    char texto[10000];
    FILE* arquivo_texto;
    
    printf("\n--- COMPACTAR ---\n");
    printf("Digite o texto que deseja compactar: ");
    fgets(texto, sizeof(texto), stdin);
    texto[strcspn(texto, "\n")] = 0;
    
    if (strlen(texto) == 0) {
        printf("❌ Nenhum texto digitado!\n");
        return;
    }
    
    // Cria o arquivo texto.txt
    arquivo_texto = fopen("texto.txt", "w");
    fprintf(arquivo_texto, "%s", texto);
    fclose(arquivo_texto);
    
    printf("\n✅ Arquivo criado: texto.txt\n");
    mostrar_conteudo("texto.txt");
    
    // Compacta
    if (compacta_arquivo("texto.txt", "texto_compactado.huf")) {
        printf("\n✅ Arquivo criado: texto_compactado.huf (compactado)\n");
        
        // Mostra o tamanho do compactado
        FILE* f = fopen("texto_compactado.huf", "rb");
        if (f) {
            fseek(f, 0, SEEK_END);
            long tamanho = ftell(f);
            printf("   Tamanho: %ld bytes\n", tamanho);
            fclose(f);
        }
    } else {
        printf("❌ ERRO na compactacao!\n");
        return;
    }
    
    printf("\n✅ COMPACTACAO REALIZADA COM SUCESSO!\n");
}

void descompactar() {
    char nome_arquivo[200];
    
    printf("\n--- DESCOMPACTAR ---\n");
    printf("Digite o nome do arquivo para descompactar: ");
    fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
    nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;
    
    if (!strstr(nome_arquivo, ".huf")) {
        strcat(nome_arquivo, ".huf");
    }
    
    // Verifica se o arquivo existe
    FILE* testa = fopen(nome_arquivo, "rb");
    if (!testa) {
        printf("❌ Arquivo '%s' nao encontrado!\n", nome_arquivo);
        return;
    }
    fclose(testa);
    
    // Descompacta
    if (descompacta_arquivo(nome_arquivo, "texto_descompactado.txt")) {
        printf("\n✅ Arquivo criado: texto_descompactado.txt\n");
        mostrar_conteudo("texto_descompactado.txt");
    } else {
        printf("❌ ERRO na descompactacao!\n");
        return;
    }
    
    printf("\n✅ DESCOMPACTACAO REALIZADA COM SUCESSO!\n");
}

void exibir_menu() {
    printf("\n========================================\n");
    printf("     COMPACTADOR / DESCOMPACTADOR       \n");
    printf("          HUFFMAN - 2026                \n");
    printf("========================================\n");
    printf("  1 - Compactar                         \n");
    printf("  2 - Descompactar                      \n");
    printf("  0 - Sair                              \n");
    printf("========================================\n");
    printf("Escolha uma opcao: ");
}

int main() {
    int opcao;
    
    printf("\n");
    printf("  ╔════════════════════════════════════════╗\n");
    printf("  ║     COMPACTADOR HUFFMAN v1.0           ║\n");
    printf("  ║     Codificacao de Huffman em C        ║\n");
    printf("  ║    Professor Andre Luis - Maligno      ║\n");
    printf("  ║   Desenvolvedores: Joao Pedro e Caio   ║\n");
    printf("  ╚════════════════════════════════════════╝\n");
    
    do {
        exibir_menu();
        scanf("%d", &opcao);
        limpar_buffer();
        
        switch (opcao) {
            case 1:
                compactar();
                break;
            case 2:
                descompactar();
                break;
            case 0:
                printf("\nPROGRAMA FINALIZADO COM SUCESSO!\n");
                break;
            default:
                printf("\n❌ Opcao invalida!\n");
        }
        
    } while (opcao != 0);
    
    return 0;
}