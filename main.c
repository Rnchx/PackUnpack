#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codigo.h"
#include "huffman.h"

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função que LÊ e MOSTRA o conteúdo de um arquivo
void mostrar_conteudo_arquivo(const char* nome) {
    FILE* f = fopen(nome, "rb");
    if (f) {
        printf("\n📄 CONTEÚDO DO ARQUIVO '%s':\n", nome);
        printf("----------------------------------------\n");
        unsigned char buffer[1000];
        size_t len = fread(buffer, 1, sizeof(buffer), f);
        
        // Tenta mostrar como texto
        printf("%.*s\n", (int)len, buffer);
        
        // Se for arquivo compactado, mostra também em hexa
        if (strstr(nome, ".huf")) {
            printf("\n(em hexadecimal): ");
            for (size_t i = 0; i < len && i < 50; i++) {
                printf("%02X ", buffer[i]);
            }
            if (len > 50) printf("...");
            printf("\n");
        }
        printf("----------------------------------------\n");
        fclose(f);
    } else {
        printf("❌ Não foi possível ler o arquivo '%s'\n", nome);
    }
}

// Função que lista arquivos usando comandos do sistema
void listar_arquivos() {
    printf("\n========================================\n");
    printf("     LISTA DE ARQUIVOS (comando ls)     \n");
    printf("========================================\n");
    system("ls -la 2>/dev/null || dir 2>nul");
    printf("========================================\n");
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
    if (!arquivo_texto) {
        printf("❌ Erro ao criar arquivo!\n");
        return;
    }
    fprintf(arquivo_texto, "%s", texto);
    fclose(arquivo_texto);
    
    // PROVA 1: Mostra o conteúdo do arquivo criado
    mostrar_conteudo_arquivo("texto.txt");
    
    // Compacta
    if (compacta_arquivo("texto.txt", "texto_compactado.huf")) {
        // PROVA 2: Mostra o conteúdo do arquivo compactado
        mostrar_conteudo_arquivo("texto_compactado.huf");
    } else {
        printf("❌ ERRO na compactacao!\n");
        return;
    }
    
    printf("\n✅ COMPACTACAO REALIZADA COM SUCESSO!\n");
    
    // PROVA 3: Lista todos os arquivos do diretório
    listar_arquivos();
}

void descompactar() {
    char nome_arquivo[200];
    
    printf("\n--- DESCOMPACTAR ---\n");
    printf("Digite o nome do arquivo para descompactar: ");
    fgets(nome_arquivo, sizeof(nome_arquivo), stdin);
    nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;
    
    // Adiciona extensão se não tiver
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
        // PROVA: Mostra o texto descompactado
        printf("\n📄 ARQUIVO DESCOMPACTADO CRIADO: texto_descompactado.txt\n");
        mostrar_conteudo_arquivo("texto_descompactado.txt");
    } else {
        printf("❌ ERRO na descompactacao!\n");
        return;
    }
    
    printf("\n✅ DESCOMPACTACAO REALIZADA COM SUCESSO!\n");
    listar_arquivos();
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
                printf("\nSaindo... Obrigado por usar o Compactador Huffman!\n");
                break;
            default:
                printf("\n❌ Opcao invalida! Tente novamente.\n");
        }
        
    } while (opcao != 0);
    
    return 0;
}