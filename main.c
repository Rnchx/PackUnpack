#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codigo.h"
#include "huffman.h"

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void limpar_tela() {
    system(CLEAR);
}

void formatar_tamanho(U64 bytes, char* buffer) {
    if (bytes < 1024) {
        sprintf(buffer, "%llu bytes", bytes);
    } else if (bytes < 1024 * 1024) {
        sprintf(buffer, "%.2f KB", (double)bytes / 1024);
    } else if (bytes < 1024 * 1024 * 1024) {
        sprintf(buffer, "%.2f MB", (double)bytes / (1024 * 1024));
    } else {
        sprintf(buffer, "%.2f GB", (double)bytes / (1024 * 1024 * 1024));
    }
}

boolean arquivo_existe(const char* nome) {
    FILE* f = fopen(nome, "rb");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
}

U64 tamanho_arquivo(const char* nome) {
    FILE* f = fopen(nome, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    U64 tamanho = ftell(f);
    fclose(f);
    return tamanho;
}

void compactar() {
    char caminho_entrada[512];
    char caminho_saida[512];
    
    printf("\n--- COMPACTAR ARQUIVO ---\n");
    printf("Caminho do arquivo a compactar: ");
    fgets(caminho_entrada, sizeof(caminho_entrada), stdin);
    caminho_entrada[strcspn(caminho_entrada, "\n")] = 0;
    
    // Remove aspas se houver
    if (caminho_entrada[0] == '"') {
        strcpy(caminho_entrada, caminho_entrada + 1);
        caminho_entrada[strlen(caminho_entrada) - 1] = '\0';
    }
    
    if (!arquivo_existe(caminho_entrada)) {
        printf("\n❌ ERRO: Arquivo '%s' nao encontrado!\n", caminho_entrada);
        printf("Pressione Enter para continuar...");
        limpar_buffer();
        return;
    }
    
    // Gera nome do arquivo de saída
    strcpy(caminho_saida, caminho_entrada);
    strcat(caminho_saida, ".huf");
    
    U64 tamanho_original = tamanho_arquivo(caminho_entrada);
    char tam_original_str[50];
    formatar_tamanho(tamanho_original, tam_original_str);
    
    printf("\n📁 Arquivo: %s", caminho_entrada);
    printf("\n📊 Tamanho original: %s", tam_original_str);
    printf("\n⏳ Compactando...\n");
    
    if (compacta_arquivo(caminho_entrada, caminho_saida)) {
        U64 tamanho_compactado = tamanho_arquivo(caminho_saida);
        char tam_compactado_str[50];
        formatar_tamanho(tamanho_compactado, tam_compactado_str);
        
        double taxa = 100.0 - (tamanho_compactado * 100.0 / tamanho_original);
        
        printf("\n✅ COMPACTACAO REALIZADA COM SUCESSO!\n");
        printf("📦 Arquivo compactado: %s\n", caminho_saida);
        printf("📊 Tamanho compactado: %s\n", tam_compactado_str);
        printf("📈 Taxa de compressão: %.2f%%\n", taxa);
    } else {
        printf("\n❌ ERRO na compactacao!\n");
    }
    
    printf("\nPressione Enter para continuar...");
    limpar_buffer();
}

void descompactar() {
    char caminho_entrada[512];
    char caminho_saida[512];
    char opcao;
    
    printf("\n--- DESCOMPACTAR ARQUIVO ---\n");
    printf("Caminho do arquivo .huf: ");
    fgets(caminho_entrada, sizeof(caminho_entrada), stdin);
    caminho_entrada[strcspn(caminho_entrada, "\n")] = 0;
    
    // Remove aspas se houver
    if (caminho_entrada[0] == '"') {
        strcpy(caminho_entrada, caminho_entrada + 1);
        caminho_entrada[strlen(caminho_entrada) - 1] = '\0';
    }
    
    if (!arquivo_existe(caminho_entrada)) {
        printf("\n❌ ERRO: Arquivo '%s' nao encontrado!\n", caminho_entrada);
        printf("Pressione Enter para continuar...");
        limpar_buffer();
        return;
    }
    
    // Verifica extensão
    if (!strstr(caminho_entrada, ".huf")) {
        printf("\n⚠️  Aviso: O arquivo nao tem extensao .huf\n");
        printf("Deseja continuar? (s/n): ");
        scanf("%c", &opcao);
        limpar_buffer();
        if (opcao != 's' && opcao != 'S') {
            return;
        }
    }
    
    printf("\nNome do arquivo de saida (Enter para nome original): ");
    fgets(caminho_saida, sizeof(caminho_saida), stdin);
    caminho_saida[strcspn(caminho_saida, "\n")] = 0;
    
    // Se não digitou nome, passa NULL para usar o nome original salvo
    const char* saida_ptr = (strlen(caminho_saida) > 0) ? caminho_saida : NULL;
    
    U64 tamanho_compactado = tamanho_arquivo(caminho_entrada);
    char tam_compactado_str[50];
    formatar_tamanho(tamanho_compactado, tam_compactado_str);
    
    printf("\n📦 Arquivo compactado: %s", caminho_entrada);
    printf("\n📊 Tamanho: %s", tam_compactado_str);
    printf("\n⏳ Descompactando...\n");
    
    if (descompacta_arquivo(caminho_entrada, saida_ptr)) {
        char nome_saida_real[512];
        if (saida_ptr) {
            strcpy(nome_saida_real, saida_ptr);
        } else {
            // Tenta descobrir o nome original (simplificado)
            strcpy(nome_saida_real, caminho_entrada);
            char* ponto = strrchr(nome_saida_real, '.');
            if (ponto && strcmp(ponto, ".huf") == 0) {
                *ponto = '\0';
            }
        }
        
        U64 tamanho_restaurado = tamanho_arquivo(nome_saida_real);
        char tam_restaurado_str[50];
        formatar_tamanho(tamanho_restaurado, tam_restaurado_str);
        
        printf("\n✅ DESCOMPACTACAO REALIZADA COM SUCESSO!\n");
        printf("📁 Arquivo restaurado: %s\n", nome_saida_real);
        printf("📊 Tamanho restaurado: %s\n", tam_restaurado_str);
    } else {
        printf("\n❌ ERRO na descompactacao!\n");
        printf("   Verifique se o arquivo foi compactado com este programa.\n");
    }
    
    printf("\nPressione Enter para continuar...");
    limpar_buffer();
}

void exibir_menu() {
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║          COMPACTADOR / DESCOMPACTADOR HUFFMAN           ║\n");
    printf("║                    Suporte a qualquer arquivo            ║\n");
    printf("╠══════════════════════════════════════════════════════════╣\n");
    printf("║                                                          ║\n");
    printf("║   📦 1 - Compactar arquivo                               ║\n");
    printf("║   📂 2 - Descompactar arquivo                            ║\n");
    printf("║   🚪 0 - Sair                                            ║\n");
    printf("║                                                          ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");
    printf("Escolha uma opcao: ");
}

int main() {
    int opcao;
    
    limpar_tela();
    printf("\n");
    printf("  ╔════════════════════════════════════════════════════╗\n");
    printf("  ║     COMPACTADOR HUFFMAN v2.0 - ARQUIVOS REAIS      ║\n");
    printf("  ║     Codificacao de Huffman em C                    ║\n");
    printf("  ║     Professor Andre Luis - Maligno                 ║\n");
    printf("  ║     Desenvolvedores: Joao Pedro e Caio             ║\n");
    printf("  ╚════════════════════════════════════════════════════╝\n");
    
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
                printf("\n✅ PROGRAMA FINALIZADO COM SUCESSO!\n");
                break;
            default:
                printf("\n❌ Opcao invalida! Tente novamente.\n");
                printf("Pressione Enter para continuar...");
                limpar_buffer();
        }
        
    } while (opcao != 0);
    
    return 0;
}