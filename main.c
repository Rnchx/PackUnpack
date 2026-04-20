#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codigo.h"
#include "huffman.h"

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para compactar um texto digitado pelo usuário
void compactar_texto() {
    char texto[10000];
    char nome_arquivo[200];
    FILE* temp;
    
    printf("\n--- COMPACTACAO ---\n");
    printf("Digite o texto que deseja compactar: ");
    fgets(texto, sizeof(texto), stdin);
    
    // Remove o \n do final
    texto[strcspn(texto, "\n")] = 0;
    
    if (strlen(texto) == 0) {
        printf("✗ Nenhum texto digitado!\n");
        return;
    }
    
    // Cria um arquivo temporário com o texto
    printf("Digite o nome do arquivo compactado (sem extensao): ");
    fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
    nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;
    
    if (strlen(nome_arquivo) == 0) {
        strcpy(nome_arquivo, "compactado");
    }
    
    // Cria arquivo temporário com o texto
    temp = fopen("temp_texto.txt", "w");
    if (!temp) {
        printf("✗ Erro ao criar arquivo temporario!\n");
        return;
    }
    fprintf(temp, "%s", texto);
    fclose(temp);
    
    // Compacta o arquivo temporário
    char saida[200];
    snprintf(saida, sizeof(saida), "%s.huf", nome_arquivo);
    
    printf("\nCompactando texto de %zu caracteres...\n", strlen(texto));
    
    if (compacta_arquivo("temp_texto.txt", saida)) {
        printf("✓ Compactacao concluida com sucesso!\n");
        printf("  Texto original: \"%s\"\n", texto);
        printf("  Tamanho original: %zu bytes (%zu bits)\n", strlen(texto), strlen(texto) * 8);
        
        // Mostra o tamanho do arquivo compactado
        FILE* f_compactado = fopen(saida, "rb");
        if (f_compactado) {
            fseek(f_compactado, 0, SEEK_END);
            long tamanho_compactado = ftell(f_compactado);
            printf("  Tamanho compactado: %ld bytes\n", tamanho_compactado);
            if (strlen(texto) > 0) {
                float taxa = (1.0 - (float)tamanho_compactado / (strlen(texto))) * 100;
                printf("  Taxa de compressao: %.1f%%\n", taxa);
            }
            fclose(f_compactado);
        }
        printf("  Arquivo gerado: %s\n", saida);
    } else {
        printf("✗ Erro na compactacao!\n");
    }
    
    // Remove o arquivo temporário
    remove("temp_texto.txt");
}

// Função para descompactar um arquivo e mostrar o texto
void descompactar_texto() {
    char nome_arquivo[200];
    char saida[200];
    FILE* f_descompactado;
    char texto[10000];
    
    printf("\n--- DESCOMPACTACAO ---\n");
    printf("Digite o nome do arquivo .huf para descompactar: ");
    fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
    nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;
    
    // Verifica se tem extensão .huf
    char* extensao = strstr(nome_arquivo, ".huf");
    if (!extensao) {
        strcat(nome_arquivo, ".huf");
    }
    
    // Verifica se o arquivo existe
    FILE* testa = fopen(nome_arquivo, "rb");
    if (!testa) {
        printf("✗ Arquivo '%s' nao encontrado!\n", nome_arquivo);
        return;
    }
    fclose(testa);
    
    // Nome do arquivo descompactado
    snprintf(saida, sizeof(saida), "temp_descompactado.txt");
    
    printf("Descompactando '%s'...\n", nome_arquivo);
    
    if (descompacta_arquivo(nome_arquivo, saida)) {
        // Lê o texto descompactado
        f_descompactado = fopen(saida, "r");
        if (f_descompactado) {
            // Lê o conteúdo do arquivo
            size_t len = fread(texto, 1, sizeof(texto) - 1, f_descompactado);
            texto[len] = '\0';
            fclose(f_descompactado);
            
            printf("✓ Descompactacao concluida com sucesso!\n");
            printf("\n========================================\n");
            printf("        TEXTO DESCOMPACTADO             \n");
            printf("========================================\n");
            printf("%s\n", texto);
            printf("========================================\n");
            printf("  Tamanho do texto: %zu caracteres\n", strlen(texto));
        } else {
            printf("✓ Descompactacao concluida, mas nao foi possivel ler o texto!\n");
        }
        
        // Remove o arquivo temporário
        remove(saida);
    } else {
        printf("✗ Erro na descompactacao! Verifique se o arquivo .huf e valido.\n");
    }
}

void exibir_menu() {
    printf("\n========================================\n");
    printf("     COMPACTADOR / DESCOMPACTADOR       \n");
    printf("          HUFFMAN - 2026                \n");
    printf("========================================\n");
    printf("  1 - Compactar um texto                \n");
    printf("  2 - Descompactar um arquivo           \n");
    printf("  0 - Sair                              \n");
    printf("========================================\n");
    printf("Escolha uma opcao: ");
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
                compactar_texto();
                break;
            case 2:
                descompactar_texto();
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