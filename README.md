# 📦 Compactador/Descompactador Huffman em C

[![Linguagem](https://img.shields.io/badge/Linguagem-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Estrutura de Dados](https://img.shields.io/badge/Estrutura-Árvore%20Binária-green.svg)]()
[![Algoritmo](https://img.shields.io/badge/Algoritmo-Huffman-orange.svg)]()

## 📋 Sobre o Projeto

Este projeto implementa o **Algoritmo de Codificação de Huffman** na linguagem C, uma técnica de compressão de dados que atribui códigos de tamanho variável aos caracteres com base em sua frequência de ocorrência. Caracteres mais frequentes recebem códigos mais curtos, resultando em uma compressão eficiente.

O programa permite:
- ✅ Compactar textos digitados pelo usuário
- ✅ Descompactar arquivos previamente compactados
- ✅ Visualizar o texto original, compactado e restaurado
- ✅ Verificar a taxa de compressão obtida

## 🎯 Objetivo do Projeto

Desenvolver um compactador/descompactador de arquivos utilizando:
- **Árvores Binárias** para representar os códigos de Huffman
- **Filas de Prioridade (Min-Heap)** para construção da árvore
- **Manipulação de bits** para geração dos arquivos compactados
- **Serialização/Deserialização** da árvore para o cabeçalho do arquivo

## 🏗️ Estrutura do Projeto
📁 projeto-huffman/
├── main.c # Interface principal com o usuário
├── huffman.c # Implementação do algoritmo de Huffman
├── huffman.h # Definições do Huffman
├── codigo.c # Estrutura de bits dinâmica
├── codigo.h # Definições da estrutura de bits
├── meustiposonlinegdb.h # Tipos de dados personalizados
└── README.md # Documentação

text

## 🚀 Como Executar

### Pré-requisitos

- Compilador GCC (MinGW no Windows ou GCC no Linux/Mac)
- Terminal ou VSCode

### Compilação

```bash
gcc -o huffman.exe main.c codigo.c huffman.c
Execução
bash
./huffman.exe
📖 Como Usar
1️⃣ Compactar um Texto
text
Escolha uma opcao: 1
Digite o texto que deseja compactar: O rato roeu a roupa do rei de Roma

✅ Arquivo criado: texto.txt
   Conteudo: "O rato roeu a roupa do rei de Roma"

✅ Arquivo criado: texto_compactado.huf (compactado)
   Tamanho: 42 bytes

✅ COMPACTACAO REALIZADA COM SUCESSO!
2️⃣ Descompactar um Arquivo
text
Escolha uma opcao: 2
Digite o nome do arquivo para descompactar: texto_compactado.huf

✅ Arquivo criado: texto_descompactado.txt
   Conteudo: "O rato roeu a roupa do rei de Roma"

✅ DESCOMPACTACAO REALIZADA COM SUCESSO!
0️⃣ Sair do Programa
🧠 Algoritmo de Huffman
Etapas do Algoritmo
Contagem de Frequências - Conta a ocorrência de cada caractere no texto

Construção da Fila de Prioridade - Insere cada caractere em uma Min-Heap

Construção da Árvore - Combina os dois nós menos frequentes até restar um nó (raiz)

Geração dos Códigos - Percorre a árvore (0 para esquerda, 1 para direita)

Compactação - Substitui cada caractere pelo seu código de Huffman

Serialização - Salva a árvore no cabeçalho do arquivo compactado

Estruturas de Dados Utilizadas
Estrutura	Finalidade
No	Nó da árvore de Huffman (simbolo, frequencia, esq, dir)
FilaPrioridade	Min-Heap para construção da árvore
Codigo	Estrutura dinâmica para manipulação de bits
LeitorBits	Leitor de bits para descompactação
📊 Exemplo de Compressão
Texto Original
text
Eerie eyes seen near lake.
Resultados
Métrica	Valor
Tamanho original	208 bits (26 bytes)
Tamanho compactado	73 bits (~9 bytes)
Taxa de compressão	64.9%
Códigos Gerados
Caractere	Código	Caractere	Código
espaço	011	e	10
E	0000	r	1100
i	0001	s	1101
y	0010	n	1110
l	0011	a	1111
k	0100	.	0101
🔧 Funcionalidades Técnicas
✅ Manipulação de bits - Escrita e leitura bit a bit

✅ Serialização da árvore - Salvando a árvore no cabeçalho do arquivo

✅ Min-Heap - Implementação completa de fila de prioridade

✅ Alocação dinâmica - Gerenciamento de memória para bits e nós

📁 Arquivos Gerados
Arquivo	Descrição
texto.txt	Texto original digitado pelo usuário
texto_compactado.huf	Arquivo compactado (formato binário)
texto_descompactado.txt	Texto restaurado após descompactação
👨‍💻 Desenvolvedores
Nome	Papel
João Pedro	Desenvolvedor
Caio	Desenvolvedor
Professor Orientador
André Luís - Maligno

📚 Disciplina
Estrutura de Dados - Codificação de Huffman

🎓 Aprendizados
Com este projeto, foi possível consolidar conhecimentos em:

Árvores binárias e suas aplicações práticas

Filas de prioridade (Min-Heap)

Manipulação de bits em C

Serialização de estruturas de dados

Compressão de dados (algoritmo de Huffman)

Desenvolvimento de software modular

📄 Licença
Este projeto foi desenvolvido para fins educacionais como parte da disciplina de Estrutura de Dados.

⭐ Como Contribuir
Faça um Fork do projeto

Crie uma branch para sua feature (git checkout -b feature/nova-feature)

Commit suas mudanças (git commit -m 'Adiciona nova feature')

Push para a branch (git push origin feature/nova-feature)

Abra um Pull Request
