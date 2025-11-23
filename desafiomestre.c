#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ITENS 50

// --- Estruturas e Enums ---

// Enum para definir os critérios de ordenação
typedef enum {
    ORDENAR_POR_NOME,
    ORDENAR_POR_TIPO,
    ORDENAR_POR_PRIORIDADE
} CriterioOrdenacao;

typedef struct {
    char nome[50];
    char tipo[30];     // Ex: Arma, Municao, Cura
    int prioridade;    // 1 (Baixa) a 5 (Alta)
} Item;

// --- Variáveis Globais para Gerenciamento ---
Item mochila[MAX_ITENS];
int qtdItens = 0;
bool ordenadoPorNome = false; // Flag para controle da Busca Binária

// --- Protótipos das Funções ---
void adicionarItem();
void listarItens();
void ordenarMochila();
void buscaBinaria();
void insertionSort(CriterioOrdenacao criterio);

// --- Função Principal ---
int main() {
    int opcao;

    do {
        printf("\n=== MOCHILA FREE FIRE (NIVEL MESTRE) ===\n");
        printf("1. Adicionar Item\n");
        printf("2. Listar Mochila\n");
        printf("3. Ordenar Itens\n");
        printf("4. Buscar Item (Busca Binaria)\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: adicionarItem(); break;
            case 2: listarItens(); break;
            case 3: ordenarMochila(); break;
            case 4: buscaBinaria(); break;
            case 0: printf("Saindo da ilha...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções ---

void adicionarItem() {
    if (qtdItens < MAX_ITENS) {
        printf("\n--- Novo Item ---\n");
        printf("Nome: ");
        scanf(" %[^\n]", mochila[qtdItens].nome); // Lê string com espaços
        
        printf("Tipo (Arma/Cura/Municao): ");
        scanf(" %[^\n]", mochila[qtdItens].tipo);

        do {
            printf("Prioridade (1-5): ");
            scanf("%d", &mochila[qtdItens].prioridade);
        } while (mochila[qtdItens].prioridade < 1 || mochila[qtdItens].prioridade > 5);

        qtdItens++;
        printf("Item adicionado com sucesso!\n");
        
        // IMPORTANTE: Ao adicionar um item, a lista pode não estar mais ordenada
        ordenadoPorNome = false; 
    } else {
        printf("Mochila cheia! Descarte algo primeiro.\n");
    }
}

void listarItens() {
    if (qtdItens == 0) {
        printf("\nMochila vazia.\n");
        return;
    }
    printf("\n--- Conteudo da Mochila ---\n");
    printf("%-20s | %-15s | %s\n", "NOME", "TIPO", "PRIORIDADE");
    printf("------------------------------------------------------\n");
    for (int i = 0; i < qtdItens; i++) {
        printf("%-20s | %-15s | %d\n", 
               mochila[i].nome, 
               mochila[i].tipo, 
               mochila[i].prioridade);
    }
}

// Menu para escolher o critério de ordenação
void ordenarMochila() {
    if (qtdItens < 2) {
        printf("Itens insuficientes para ordenar.\n");
        return;
    }

    int escolha;
    printf("\n--- Criterio de Ordenacao ---\n");
    printf("1. Nome (A-Z)\n");
    printf("2. Tipo (A-Z)\n");
    printf("3. Prioridade (1-5)\n");
    printf("Escolha: ");
    scanf("%d", &escolha);

    switch (escolha) {
        case 1: 
            insertionSort(ORDENAR_POR_NOME); 
            ordenadoPorNome = true; // Habilita busca binária
            break;
        case 2: 
            insertionSort(ORDENAR_POR_TIPO); 
            ordenadoPorNome = false; // Desabilita busca binária por nome
            break;
        case 3: 
            insertionSort(ORDENAR_POR_PRIORIDADE); 
            ordenadoPorNome = false; // Desabilita busca binária por nome
            break;
        default: printf("Opcao invalida.\n");
    }
}

// Algoritmo Insertion Sort com contador de comparações
void insertionSort(CriterioOrdenacao criterio) {
    int i, j;
    Item key;
    long comparacoes = 0;

    for (i = 1; i < qtdItens; i++) {
        key = mochila[i];
        j = i - 1;

        bool trocar = false;

        // Loop interno para encontrar a posição correta
        // Note: A condição do while verifica se j >= 0 E se deve trocar
        while (j >= 0) {
            comparacoes++; // Contabiliza a comparação
            
            switch (criterio) {
                case ORDENAR_POR_NOME:
                    if (strcmp(mochila[j].nome, key.nome) > 0) trocar = true;
                    else trocar = false;
                    break;
                case ORDENAR_POR_TIPO:
                    if (strcmp(mochila[j].tipo, key.tipo) > 0) trocar = true;
                    else trocar = false;
                    break;
                case ORDENAR_POR_PRIORIDADE:
                    // Ordena do menor (1) para o maior (5)
                    if (mochila[j].prioridade > key.prioridade) trocar = true;
                    else trocar = false;
                    break;
            }

            if (trocar) {
                mochila[j + 1] = mochila[j];
                j = j - 1;
            } else {
                break; // Se não precisa trocar, sai do loop interno
            }
        }
        mochila[j + 1] = key;
    }

    printf("\nOrdenacao concluida! Comparacoes realizadas: %ld\n", comparacoes);
}

// Algoritmo de Busca Binária (Requer lista ordenada por Nome)
void buscaBinaria() {
    // Validação de Pré-requisito
    if (!ordenadoPorNome) {
        printf("\n[ERRO] A mochila NAO esta ordenada por nome.\n");
        printf("Execute a ordenacao por Nome antes de buscar.\n");
        return;
    }

    char alvo[50];
    printf("\nDigite o nome do item para buscar: ");
    scanf(" %[^\n]", alvo);

    int inicio = 0;
    int fim = qtdItens - 1;
    int meio;
    bool encontrado = false;
    int comparacoesBusca = 0;

    while (inicio <= fim) {
        meio = (inicio + fim) / 2;
        comparacoesBusca++;

        int resultado = strcmp(mochila[meio].nome, alvo);

        if (resultado == 0) {
            printf("\n--- ITEM ENCONTRADO ---\n");
            printf("Nome: %s\n", mochila[meio].nome);
            printf("Tipo: %s\n", mochila[meio].tipo);
            printf("Prioridade: %d\n", mochila[meio].prioridade);
            printf("Indice na mochila: %d\n", meio);
            printf("Comparacoes na busca: %d\n", comparacoesBusca);
            encontrado = true;
            break;
        }
        else if (resultado < 0) {
            inicio = meio + 1; // Está na metade superior
        }
        else {
            fim = meio - 1; // Está na metade inferior
        }
    }

    if (!encontrado) {
        printf("\nItem '%s' nao encontrado na mochila.\n", alvo);
    }
}