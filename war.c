// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>

#define MAX_TERRITORIOS 5
#define MAX_MISSOES 10
#define MAX_STRING 50

typedef struct {
    char nome[30];
    char corExercito[10];
    int numTropas;
} Territorio;

// Protótipos das funções
bool alocarMapa(Territorio **mapa);
void inicializarTerritorios(Territorio *mapa);
void liberarMemoria(Territorio *mapa);
void exibirMenuPrincipal();
void exibirMapa(const Territorio *mapa);
void exibirMissao(int missaoID);
void faseDeAtaque(Territorio *mapa);
void simularAtaque(Territorio *origem, Territorio *destino);
int sortearMissao();
int verificarVitoria(const Territorio *mapa, int missaoID);
void limparBufferEntrada();

int main() {
    Territorio *mapaPtr = NULL;
    int escolha;

    // Configura locale para português
    setlocale(LC_ALL, "Portuguese");

    // Inicializa semente para números aleatórios
    srand((unsigned int)time(NULL));

    // Aloca mapa
    if (!alocarMapa(&mapaPtr)) {
        fprintf(stderr, "Erro ao alocar memoria para o mapa.\n");
        return 1;
    }

    // Inicializa territórios
    inicializarTerritorios(mapaPtr);

    int missaoID = sortearMissao();

    printf("Bem-Vindo ao war Estruturado - Desafio de Codigo!\n");
    printf("Sua missao secreta e: ");
    exibirMissao(missaoID);

    do {
        exibirMapa(mapaPtr);
        exibirMenuPrincipal();
        printf("Escolha uma acao: ");
        if (scanf("%d", &escolha) != 1) {
            printf("Entrada invalida. Tente novamente.\n");
            limparBufferEntrada();
            continue;
        }
        limparBufferEntrada();

        switch (escolha) {
            case 1:
                faseDeAtaque(mapaPtr);
                break;
            case 2:
                if (verificarVitoria(mapaPtr, missaoID)) {
                    printf("Parabens! Voce cumpriu sua missao e venceu o jogo!\n");
                    escolha = 0;
                } else {
                    printf("Missao ainda nao cumprida. Continue jogando!\n");
                }
                break;
            case 0:
                printf("Encerrando o jogo. Obrigado por jogar!\n");
                break;
            default:
                printf("Opcao invalida. Por favor, escolha novamente.\n");
        }
        if (escolha != 0) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }

    } while (escolha != 0);

    liberarMemoria(mapaPtr);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
bool alocarMapa(Territorio **mapa) {
    *mapa = (Territorio *)calloc(MAX_TERRITORIOS, sizeof(Territorio));
    return (*mapa != NULL); // Retorna verdadeiro se a alocação foi bem-sucedida, falso caso contrário.
}
// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void inicializarTerritorios(Territorio *mapa) {
    // Exemplo de inicialização de territórios, pode ser expandida para incluir mais territórios e dados variados.
    strcpy(mapa[0].nome, "Território A");
    strcpy(mapa[0].corExercito, "Vermelho");
    mapa[0].numTropas = 3;
    
    strcpy(mapa[1].nome, "Território B");
    strcpy(mapa[1].corExercito, "Azul");
    mapa[1].numTropas = 2;

    strcpy(mapa[2].nome, "Território C");
    strcpy(mapa[2].corExercito, "Verde");
    mapa[2].numTropas = 4;

    strcpy(mapa[3].nome, "Território D");
    strcpy(mapa[3].corExercito, "Amarelo");
    mapa[3].numTropas = 1;

    strcpy(mapa[4].nome, "Território E");
    strcpy(mapa[4].corExercito, "Laranja");
    mapa[4].numTropas = 5;
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(Territorio *mapa) {
    free(mapa); // Libera a memória alocada para o mapa.
}

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.
void exibirMenuPrincipal() {
    printf("Menu Principal: \n");
    printf("1. Atacar\n");
    printf("2. Verificar Missão\n");
    printf("0. Sair\n");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const Territorio *mapa) {
    printf("\nMapa do Mundo:\n");
    printf("%-15s %-10s %-10s\n", "Território", "Cor", "Tropas");
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        printf("%-15s %-10s %-10d\n", mapa[i].nome, mapa[i].corExercito, mapa[i].numTropas);
    }
}

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.
void exibirMissao(int missaoID) {
    // Exemplo de missões, podee ser expandida para incluir mais missões e descrições variadas.
    switch (missaoID) {
        case 0:
            printf("Conquistar 3 territórios.\n");
            break;
        case 1:
            printf("Destruir o exército Azul.\n");
            break;
        case 2:
            printf("Conquistar o território C.\n");
            break;
        default:
            printf("Missão desconhecida.\n");  
    }
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.
void faseDeAtaque(Territorio *mapa) {
    int origem, destino;
    printf("Digite o número do território de origem (0-%d): ", MAX_TERRITORIOS - 1);
    scanf("%d", &origem);
    printf("Digite o número do território de destino (0-%d): ", MAX_TERRITORIOS - 1);
    scanf("%d", &destino);
    limparBufferEntrada(); // Limpa o buffer para evitar problemas com leituras subsequentes.
    if (origem >= 0 && origem < MAX_TERRITORIOS && destino >= 0 && destino < MAX_TERRITORIOS) {
        simularAtaque(&mapa[origem], &mapa[destino]); // Chama a função para simular o ataque entre os territórios selecionados.
    } else {
        printf("Territórios inválidos. Tente novamente.\n");
    }
}

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void simularAtaque(Territorio *origem, Territorio *destino) {
    if (origem->numTropas < 2) {
        printf("Território de origem deve ter pelo menos 2 tropas para atacar.\n");
        return;
    }
    if (strcmp(origem->corExercito, destino->corExercito) == 0) {
        printf("Não é possível atacar um território do mesmo exército.\n");
        return;
    }
    int dadosOrigem = rand() % 6 + 1; // Rola um dado para o território de origem (1-6).
    int dadosDestino = rand() % 6 + 1; // Rola um dado para o território de destino (1-6).
    printf("Dados do território de origem: %d\n", dadosOrigem);
    printf("Dados do território de destino: %d\n", dadosDestino);
    if (dadosOrigem > dadosDestino) {
        printf("Ataque bem-sucedido! O território de destino perde uma tropa;\n");
        destino->numTropas--;
        if (destino->numTropas == 0) {
            printf("Território conquistado! O território de destino agora pertence ao exército do território de origem;\n");
            strcpy(destino->corExercito, origem->corExercito); // Atualiza o dono do território conquistado.
            destino->numTropas = 1; // Move uma tropa do território de origem para o território conquistado.
            origem->numTropas--;
        } else {
            printf("O território de destino ainda resiste ao ataque.\n");
        }
    }
}


// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.
int sortearMissao() {
    return rand() % 3; // Retorna um número aleatório entre 0 e 2, representando o ID da missão.
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.
int verificarVitoria(const Territorio *mapa, int missaoID) {
    switch (missaoID) {
        case 0: { // Conquistar 3 territórios
            int count = 0;
            for (int i = 0; i < MAX_TERRITORIOS; i++) {
                if (strcmp(mapa[i].corExercito, "Vermelho") == 0) count++;
            }
            return count >= 3;
        }
        case 1: { // Destruir o exército Azul
            for (int i = 0; i < MAX_TERRITORIOS; i++) {
                if (strcmp(mapa[i].corExercito, "Azul") == 0) return 0;
            }
            return 1;
        }
        case 2: { // Conquistar o território C
            return strcmp(mapa[2].corExercito, "Vermelho") == 0;
        }
        default:
            return 0;
    }
}

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Lê e descarta caracteres até encontrar uma nova linha ou o fim do arquivo.
}