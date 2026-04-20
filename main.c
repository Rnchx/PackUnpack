#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codigo.h"
#include "huffman.h"

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibir_menu() {
    printf("\n========================================\n");
    printf("     COMPACTADOR / DESCOMPACTADOR       \n");
    printf("          HUFFMAN - 2026                \n");
    printf("========================================\n");
    printf("  1 - Compactar arquivo                 \n");
    printf("  2 - Descompactar arquivo              \n");
    printf("  0 - Sair                              \n");
    printf("========================================\n");
    printf("Escolha uma opcao: ");
}

void compactar() {
    char nome_arquivo[200];
    char nome_saida[200];
    
    printf("\n--- COMPACTACAO ---\n");
    printf("Digite o nome do arquivo que deseja compactar: ");
    fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
    nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;  // remove o \n
    
    // Gera nome do arquivo de saida
    snprintf(nome_saida, sizeof(nome_saida), "%s.huf", nome_arquivo);
    
    printf("Compactando '%s' para '%s'...\n", nome_arquivo, nome_saida);
    
    if (compacta_arquivo(nome_arquivo, nome_saida)) {
        printf("✓ Compactacao concluida com sucesso!\n");
        
        // Mostra informações do arquivo original e compactado
        FILE* f_original = fopen(nome_arquivo, "rb");
        FILE* f_compactado = fopen(nome_saida, "rb");
        
        if (f_original && f_compactado) {
            fseek(f_original, 0, SEEK_END);
            fseek(f_compactado, 0, SEEK_END);
            long tamanho_original = ftell(f_original);
            long tamanho_compactado = ftell(f_compactado);
            
            printf("  Tamanho original: %ld bytes\n", tamanho_original);
            printf("  Tamanho compactado: %ld bytes\n", tamanho_compactado);
            if (tamanho_original > 0) {
                float taxa = (1.0 - (float)tamanho_compactado / tamanho_original) * 100;
                printf("  Taxa de compressao: %.1f%%\n", taxa);
            }
            
            fclose(f_original);
            fclose(f_compactado);
        }
    } else {
        printf("✗ Erro na compactacao! Verifique se o arquivo existe.\n");
    }
}

void descompactar() {
    char nome_arquivo[200];
    char nome_saida[200];
    
    printf("\n--- DESCOMPACTACAO ---\n");
    printf("Digite o nome do arquivo .huf que deseja descompactar: ");
    fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
    nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;
    
    // Verifica se tem extensão .huf, se não tiver, adiciona
    char* extensao = strstr(nome_arquivo, ".huf");
    if (!extensao) {
        strcat(nome_arquivo, ".huf");
    }
    
    // Gera nome do arquivo de saida (remove .huf e adiciona .txt ou _restaurado)
    char nome_base[200];
    strcpy(nome_base, nome_arquivo);
    char* ponto = strstr(nome_base, ".huf");
    if (ponto) {
        *ponto = '\0';
    }
    snprintf(nome_saida, sizeof(nome_saida), "%s_restaurado.txt", nome_base);
    
    printf("Descompactando '%s' para '%s'...\n", nome_arquivo, nome_saida);
    
    if (descompacta_arquivo(nome_arquivo, nome_saida)) {
        printf("✓ Descompactacao concluida com sucesso!\n");
        printf("  Arquivo gerado: %s\n", nome_saida);
    } else {
        printf("✗ Erro na descompactacao! Verifique se o arquivo .huf existe e é valido.\n");
    }
}

int main() {
    int opcao;
    
    printf("\n");
    printf("  ╔════════════════════════════════════════╗\n");
    printf("  ║     COMPACTADOR HUFFMAN v1.0           ║\n");
    printf("  ║                                        ║\n");
    printf("  ║     Codificacao de Huffman em C        ║\n");
    printf("  ║    Professor André Luís - Maligno      ║\n");
    printf("  ║  Professor André Luís - Maligno        ║\n");
    printf("  ╚════════════════════════════════════════╝\n");
    
    do {
        exibir_menu();
        scanf("%d", &opcao);
        limpar_buffer();  // limpa o buffer do teclado
        
        switch (opcao) {
            case 1:
                compactar();
                break;
            case 2:
                descompactar();
                break;
            case 0:
                printf("\nSaindo... Obrigado por usar o Compactador Huffman!\n");
                break;
            default:
                printf("\n✗ Opcao invalida! Tente novamente.\n");
        }
        
    } while (opcao != 0);
    
    return 0;
}