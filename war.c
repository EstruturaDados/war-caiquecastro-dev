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

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

#define MAX_TERRITORIOS 5
#define MAX_MISSOES 10
#define MAX_STRING 50
// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[30];
    char corExercito[10];
    int numTropas;
} Territorio;
// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
bool alocarMapa(Territorio **mapa);
void inicializarTerritorios(Territorio *mapa);
// Funções de setup e gerenciamento de memória:
void liberarMemoria(Territorio *mapa);
// Funções de interface com o usuário:
void exibirMenuPrincipal();
// Funções de lógica principal do jogo:
void exibirMapa(const Territorio *mapa);
// Função utilitária:
void exibirMissao(int missaoID);
void faseDeAtaque(Territorio *mapa);
void simularAtaque(Territorio *origem, Territorio *destino);
int sortearMissao();
int verificarVitoria (const Territorio *mapa, int missaoID);
void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    struct Territorio[MAX_TERRITORIOS] mapa; // Declaração do mapa do mundo como um array de territórios.
    int totalTerritorios = 0; // Variavel para contar o número total de territórios, usada para controle e validação.
    int escolha; // Variável para armazenar a escolha do jogador no menu.
    // 1. Configuração Inicial (Setup):
    Territorio *mapaPtr = NULL; // Ponteiro para o mapa, usado para alocação dinâmica.
    // - Define o locale para português.
    setLocale(LC_ALL, "Portuguese");
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    void inicializarSemente() {
        srand(time(NULL));
    }
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    if (!alocarMapa(&mapaPtr)) {
        fprintf(stderr, "Erro ao alocar memória para o mapa.\n");
        return 1; // Encerra o programa com código de erro.
    }
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    inicializarTerritorios(mapaPtr);
    // - Define a cor do jogador e sorteia sua missão secreta.
    char corJogador[MAX_STRING] = "Vermelho"; // Cor do exécito do jogador, usada para identificar seus territórios.
    int missaoID = sortearMissao(); // Sorteia um ID de missão para o jogador, que será usado para verificar a vitória posteriormente.
    // 2. Laço Principal do Jogo (Game Loop):
    totalTerritorios = MAX_TERRITORIOS;
    printf("Bem-Vindo ao war Estruturado - Desafio de Código!\n");
    printf("Sua missão secreta é: ");
    exibirMissao(missaoID); // Exibe a missão sorteada para o jogador.
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
        do {
            exibirMapa(mapaPtr); // Exibe o estado atual do mapa a cada rodada.
            exibirMenuPrincipal(); // Mostra as opções de ações para o jogador.
            printf("Escolha uma ação: ");
            scanf("%d", &escolha); // Lê a escolha do jogador.
            limparBufferEntrada(); // Limpa o buffer de entrada para evitar problemas com leituras subsequentes.
            switch (escolha) {
                case 1:
                    faseDeAtaque(mapaPtr); // Inicia a fase de ataque, onde o jogador pode escolher territórios para atacar.
                    break;
                case 2:
                    if (verificarVitoria(mapaPtr, missaoID)) {
                        printf("Parabéns! Você cumpriu sua missão e venceu o jogo!\n");
                        escolha = 0; // Define escolha para 0 para sair do loop e encerrar o jogo.
                    } else {
                        printf("Missão ainda não cumprida. Continue jogando!\n");
                    }
                    break;
                case 0:
                    printf("Encerrando o jogo. Obrigado por jogar!\n");
                    break;
                default:
                    printf("Opção inválida. Por favor, escolha novamente.\n");
            }
            printf("\nPressione Enter para continuar...");
            getchar(); // Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.
        } while (escolha != 0);
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.

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
    // Exemplo de inicialização de terriórios, pode ser expandida para incluir mais territórios e dados variados.
    strcpy(mapa[0].nome, "Território A");
    strcpy(mapa[0].corExercito, "Vermelho");
    mapa[0].numTropas = 3;
    
    strcpy([1].nome, "Território B");
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
            strcpy(destino->corExercito, origem->corExercito); // Atual o dono do território conquistado.
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
    return rand() % MAX_MISSOES; // Retorna um número aleatório entre 0 e MAX_MISSOES-1, representando o ID da missão.
}

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
