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
#include <time.h>
#include <locale.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define NUM_TERRITORIOS 10
#define NUM_MISSOES 2
#define MAX_NOME 30
#define TERRITORIOS_PARA_VENCER 7

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_NOME];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// Funções de setup e gerenciamento de memória:
Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio** mapa);

// Funções de interface com o usuário:
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa);
void exibirMissao(int idMissao, const char* corAlvo);

// Funções de lógica principal do jogo:
void faseDeAtaque(Territorio* mapa, const char* corJogador);
void simularAtaque(Territorio* mapa, int idOrigem, int idDestino, const char* corJogador);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, const char* corJogador, int idMissao, const char* corAlvo);

// Função utilitária:
void limparBufferEntrada();

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL));

    Territorio* mundo = alocarMapa();
    if (mundo == NULL) {
        printf("Erro: Falha ao alocar memória para o mapa do mundo.\n");
        return 1; // Encerra o programa com código de erro
    }

    inicializarTerritorios(mundo);
    const char* corJogador = "Verde";
    const char* corAlvoMissao = "Vermelho"; // Cor a ser destruída na missão 0
    int missaoJogador = sortearMissao();
    int escolha;
    int vitoria = 0;

    // 2. Laço Principal do Jogo (Game Loop):
    do {
        // Limpa a tela (pode ser "clear" em sistemas Unix/Linux)
        system("clear"); 
        
        exibirMapa(mundo);
        exibirMissao(missaoJogador, corAlvoMissao);
        exibirMenuPrincipal();

        printf("Escolha sua ação: ");
        scanf("%d", &escolha);
        limparBufferEntrada();

        switch (escolha) {
            case 1:
                faseDeAtaque(mundo, corJogador);
                break;
            case 2:
                vitoria = verificarVitoria(mundo, corJogador, missaoJogador, corAlvoMissao);
                if (vitoria) {
                    printf("\n*** PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO E VENCEU O JOGO! ***\n");
                } else {
                    printf("\nVocê ainda não cumpriu sua missão. Continue lutando!\n");
                }
                break;
            case 0:
                printf("\nEncerrando o jogo. Até a próxima!\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
                break;
        }

        if (escolha != 0) {
            printf("\nPressione Enter para continuar...");
            getchar(); // Pausa a execução
        }

    } while (escolha != 0 && !vitoria);

    // 3. Limpeza:
    liberarMemoria(&mundo);
    printf("Memória liberada com sucesso.\n");

    return 0;
}

// --- Implementação das Funções ---

/**
 * @brief Aloca dinamicamente a memória para o vetor de territórios.
 * @return Ponteiro para o mapa alocado ou NULL em caso de erro.
 */
Territorio* alocarMapa() {
    return (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

/**
 * @brief Preenche os dados iniciais de cada território.
 * @param mapa Ponteiro para o mapa do mundo a ser modificado.
 */
void inicializarTerritorios(Territorio* mapa) {
    const char* nomes[NUM_TERRITORIOS] = {
        "Brasil", "Argentina", "Peru", "Venezuela", "Mexico",
        "Canada", "Alasca", "California", "Texas", "Groelandia"
    };
    const char* cores[] = {"Verde", "Vermelho", "Azul"};

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, cores[rand() % 3]); // Sorteia uma das 3 cores
        mapa[i].tropas = (rand() % 5) + 1;      // Sorteia de 1 a 5 tropas
    }
}

/**
 * @brief Libera a memória alocada para o mapa.
 * @param mapa Ponteiro para o ponteiro do mapa, para que possa ser setado para NULL.
 */
void liberarMemoria(Territorio** mapa) {
    if (mapa != NULL && *mapa != NULL) {
        free(*mapa);
        *mapa = NULL; // Evita "dangling pointers"
    }
}

/**
 * @brief Imprime o menu principal de ações do jogador.
 */
void exibirMenuPrincipal() {
    printf("\n--- MENU DE AÇÕES ---\n");
    printf("1. Atacar um território\n");
    printf("2. Verificar missão\n");
    printf("0. Sair do jogo\n");
}

/**
 * @brief Exibe o estado atual de todos os territórios no mapa.
 * @param mapa Ponteiro constante para o mapa, garantindo que não será modificado.
 */
void exibirMapa(const Territorio* mapa) {
    printf("==================== MAPA DO MUNDO ====================\n");
    printf("| ID | Território      | Dono (Cor)   | Tropas |\n");
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("| %-2d | %-15s | %-12s | %-6d |\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("=======================================================\n");
}

/**
 * @brief Exibe a missão secreta do jogador.
 * @param idMissao O ID da missão sorteada.
 * @param corAlvo A cor do exército a ser destruído (relevante para a missão 0).
 */
void exibirMissao(int idMissao, const char* corAlvo) {
    printf("\n--- SUA MISSÃO SECRETA ---\n");
    switch (idMissao) {
        case 0:
            printf("Destruir todos os exércitos da cor: %s\n", corAlvo);
            break;
        case 1:
            printf("Conquistar %d territórios no total.\n", TERRITORIOS_PARA_VENCER);
            break;
    }
}

/**
 * @brief Gerencia a interface da fase de ataque.
 * @param mapa Ponteiro para o mapa do mundo a ser modificado.
 * @param corJogador String constante com a cor do jogador.
 */
void faseDeAtaque(Territorio* mapa, const char* corJogador) {
    int idOrigem, idDestino;
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Digite o ID do seu território de origem: ");
    scanf("%d", &idOrigem);
    printf("Digite o ID do território de destino para atacar: ");
    scanf("%d", &idDestino);
    limparBufferEntrada();
    
    simularAtaque(mapa, idOrigem, idDestino, corJogador);
}

/**
 * @brief Simula uma batalha entre dois territórios com validações.
 * @param mapa Ponteiro para o mapa do mundo a ser modificado.
 * @param idOrigem Índice do território de ataque.
 * @param idDestino Índice do território de defesa.
 * @param corJogador String constante com a cor do jogador.
 */
void simularAtaque(Territorio* mapa, int idOrigem, int idDestino, const char* corJogador) {
    // Validações
    if (idOrigem < 0 || idOrigem >= NUM_TERRITORIOS || idDestino < 0 || idDestino >= NUM_TERRITORIOS) {
        printf("IDs de território inválidos!\n");
        return;
    }
    if (strcmp(mapa[idOrigem].cor, corJogador) != 0) {
        printf("Ataque inválido! Você não domina o território de origem.\n");
        return;
    }
    if (strcmp(mapa[idDestino].cor, corJogador) == 0) {
        printf("Ataque inválido! Você já domina o território de destino.\n");
        return;
    }
    if (mapa[idOrigem].tropas < 2) {
        printf("Ataque inválido! Você precisa de pelo menos 2 tropas para atacar.\n");
        return;
    }

    // Lógica da Batalha (simplificada)
    printf("\nBatalha: %s (%d tropas) ataca %s (%d tropas)!\n", mapa[idOrigem].nome, mapa[idOrigem].tropas, mapa[idDestino].nome, mapa[idDestino].tropas);
    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;
    printf("Dado de ataque: %d | Dado de defesa: %d\n", dadoAtaque, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("Vitória do atacante! O defensor perdeu 1 tropa.\n");
        mapa[idDestino].tropas--;
    } else {
        printf("Vitória do defensor! O atacante perdeu 1 tropa.\n");
        mapa[idOrigem].tropas--;
    }

    // Lógica de Conquista
    if (mapa[idDestino].tropas == 0) {
        printf("Território %s foi conquistado!\n", mapa[idDestino].nome);
        strcpy(mapa[idDestino].cor, corJogador);
        mapa[idDestino].tropas = 1;
        mapa[idOrigem].tropas--;
        printf("Uma tropa foi movida para o novo território.\n");
    }
}

/**
 * @brief Sorteia um ID de missão aleatório.
 * @return O ID da missão (0 ou 1).
 */
int sortearMissao() {
    return rand() % NUM_MISSOES;
}

/**
 * @brief Verifica se o jogador cumpriu sua missão.
 * @param mapa Ponteiro constante para o mapa do mundo.
 * @param corJogador String constante com a cor do jogador.
 * @param idMissao O ID da missão do jogador.
 * @param corAlvo A cor do exército a ser destruído.
 * @return 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarVitoria(const Territorio* mapa, const char* corJogador, int idMissao, const char* corAlvo) {
    switch (idMissao) {
        case 0: // Destruir um exército
        {
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor, corAlvo) == 0) {
                    return 0; // Se encontrar ao menos 1 território do alvo, não venceu
                }
            }
            return 1; // Se o loop terminar, o alvo foi eliminado
        }
        case 1: // Conquistar um número de territórios
        {
            int contagem = 0;
            for (int i = 0; i < NUM_TERRITORIOS; i++) {
                if (strcmp(mapa[i].cor, corJogador) == 0) {
                    contagem++;
                }
            }
            return contagem >= TERRITORIOS_PARA_VENCER; // Retorna verdadeiro se a contagem for suficiente
        }
    }
    return 0; // Caso padrão
}

/**
 * @brief Limpa o buffer de entrada do teclado para evitar erros de leitura.
 */
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}