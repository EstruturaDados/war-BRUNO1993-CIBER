// ==========================================================
// Autor: Bruno Machado
// Data: 16NOV2025
// Disciplina: Estrutura de Dados
// Projeto: Jogo de Estratégia WAR - Nível Mestre
// ==========================================================
// Descrição:
// Implementação do jogo de tabuleiro WAR em C, utilizando
// alocação dinâmica, ponteiros e modularização. O jogo
// inclui um sistema de batalha por dados e missões secretas.
// ==========================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

// Constantes que definem as regras do jogo
#define NUM_TERRITORIOS 10
#define NUM_MISSOES 2
#define MAX_NOME 30
#define TERRITORIOS_PARA_VENCER 7

// A struct que representa cada território no nosso mapa
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_NOME];
    int tropas;
} Territorio;

// --- Protótipos das Funções ---
// Organizando as declarações de todas as funções que vamos usar
Territorio* alocarMapa();
void inicializarTerritorios(Territorio* mapa);
void liberarMemoria(Territorio** mapa);
void exibirMenuPrincipal();
void exibirMapa(const Territorio* mapa);
void exibirMissao(int idMissao, const char* corAlvo);
void faseDeAtaque(Territorio* mapa, const char* corJogador);
void simularAtaque(Territorio* mapa, int idOrigem, int idDestino, const char* corJogador);
int sortearMissao();
int verificarVitoria(const Territorio* mapa, const char* corJogador, int idMissao, const char* corAlvo);
void limparBufferEntrada(); // Função essencial para lidar com o scanf

// --- Função Principal (main) ---
int main() {
    // Define o idioma para português, para acentos e "ç"
    setlocale(LC_ALL, "Portuguese");
    // Inicializa o gerador de números aleatórios com uma semente baseada no tempo
    srand(time(NULL));

    // 1. Setup inicial do jogo
    Territorio* mapa = alocarMapa();
    if (mapa == NULL) {
        printf("Erro crítico: Falha ao alocar memória para o mapa.\n");
        return 1;
    }

    inicializarTerritorios(mapa);
    const char* corJogador = "Verde";
    const char* missao_cor_alvo = "Vermelho"; // Missão de destruir exército
    int missaoJogador = sortearMissao();
    int opcao;
    int vitoria = 0; // Flag para controlar o fim do jogo

    // 2. Laço Principal do Jogo (Game Loop)
    do {
        system("clear"); // Limpa a tela do terminal (funciona em Linux EU NAO TESTEI NO WINDOWS)
        
        exibirMapa(mapa);
        exibirMissao(missaoJogador, missao_cor_alvo);
        exibirMenuPrincipal();

        printf("Escolha sua ação: ");
        scanf("%d", &opcao);
        // É crucial limpar o buffer depois de ler um número com scanf,
        // senão o "Enter" (\n) fica no buffer e atrapalha a próxima leitura.
        limparBufferEntrada();

        switch (opcao) {
            case 1:
                faseDeAtaque(mapa, corJogador);
                break;
            case 2:
                vitoria = verificarVitoria(mapa, corJogador, missaoJogador, missao_cor_alvo);
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

        // Pausa o jogo para que o jogador possa ler o resultado da sua ação
        if (opcao != 0 && !vitoria) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }

    } while (opcao != 0 && !vitoria);

    // 3. Limpeza
    // Sempre liberar a memória que foi alocada dinamicamente
    liberarMemoria(&mapa);
    printf("Memória liberada com sucesso.\n");

    return 0;
}

// --- Implementação das Funções ---

// Função para criar o mapa do mundo na memória usando calloc.
// Calloc já inicializa a memória com zeros, o que é uma boa prática.
Territorio* alocarMapa() {
    return (Territorio*)calloc(NUM_TERRITORIOS, sizeof(Territorio));
}

// Preenche os territórios com dados iniciais aleatórios.
// Assim cada jogo começa de um jeito diferente.
void inicializarTerritorios(Territorio* mapa) {
    const char* nomes[NUM_TERRITORIOS] = {
        "Brasil", "Argentina", "Peru", "Venezuela", "Mexico",
        "Canada", "Alasca", "California", "Texas", "Groelandia"
    };
    const char* cores[] = {"Verde", "Vermelho", "Azul"};

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, cores[rand() % 3]); // Sorteia uma cor
        mapa[i].tropas = (rand() % 5) + 2;      // Sorteia de 2 a 6 tropas
    }
}

// Libera a memória alocada para o mapa.
// Recebe um ponteiro para ponteiro (**) para poder alterar o ponteiro original para NULL.
// Isso evita "dangling pointers" (ponteiros que apontam para memória já liberada).
void liberarMemoria(Territorio** mapa) {
    if (mapa != NULL && *mapa != NULL) {
        free(*mapa);
        *mapa = NULL;
    }
}

// Apenas imprime o menu de opções para o jogador.
void exibirMenuPrincipal() {
    printf("\n--- MENU DE AÇÕES ---\n");
    printf("1. Atacar um território\n");
    printf("2. Verificar missão\n");
    printf("0. Sair do jogo\n");
}

// Mostra o estado atual de todos os territórios.
// O 'const' aqui é uma garantia de que a função só vai ler os dados, sem modificar o mapa.
void exibirMapa(const Territorio* mapa) {
    printf("==================== MAPA DO MUNDO ====================\n");
    printf("| ID | Território      | Dono (Cor)   | Tropas |\n");
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("| %-2d | %-15s | %-12s | %-6d |\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("=======================================================\n");
}

// Mostra a descrição da missão secreta do jogador.
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

// Controla a interface para o jogador escolher o ataque.
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

// Contém a lógica de uma batalha entre dois territórios.
void simularAtaque(Territorio* mapa, int idOrigem, int idDestino, const char* corJogador) {
    // 1. Validar se o ataque é possível
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

    // 2. Simular a batalha com dados
    printf("\nBatalha: %s (%d tropas) ataca %s (%d tropas)!\n", mapa[idOrigem].nome, mapa[idOrigem].tropas, mapa[idDestino].nome, mapa[idDestino].tropas);
    int dadoAtaque = rand() % 6 + 1;
    int dadoDefesa = rand() % 6 + 1;
    printf("Dado de ataque: %d | Dado de defesa: %d\n", dadoAtaque, dadoDefesa);

    // Regra: O atacante só vence se o dado for estritamente maior. Empate favorece a defesa.
    if (dadoAtaque > dadoDefesa) {
        printf("Vitória do atacante! O defensor perdeu 1 tropa.\n");
        mapa[idDestino].tropas--;
    } else {
        printf("Vitória do defensor! O atacante perdeu 1 tropa.\n");
        mapa[idOrigem].tropas--;
    }

    // 3. Verificar se o território foi conquistado
    if (mapa[idDestino].tropas == 0) {
        printf("Território %s foi conquistado!\n", mapa[idDestino].nome);
        strcpy(mapa[idDestino].cor, corJogador);
        mapa[idDestino].tropas = 1; // O atacante move 1 tropa para o novo território
        mapa[idOrigem].tropas--;
        printf("Uma tropa foi movida para o novo território.\n");
    }
}

// Sorteia um ID de missão aleatório para o jogador no início do jogo.
int sortearMissao() {
    return rand() % NUM_MISSOES;
}

// Verifica se o jogador cumpriu sua missão.
// Retorna 1 (verdadeiro) se venceu, ou 0 (falso) se não.
int verificarVitoria(const Territorio* mapa, const char* corJogador, int idMissao, const char* corAlvo) {
    if (idMissao == 0) { // Missão: Destruir um exército
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, corAlvo) == 0) {
                return 0; // Se encontrar ao menos 1 território do alvo, a missão não foi cumprida
            }
        }
        return 1; // Se o loop terminar, significa que o alvo foi eliminado
    } else { // Missão: Conquistar N territórios
        int contagem = 0;
        for (int i = 0; i < NUM_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                contagem++;
            }
        }
        return contagem >= TERRITORIOS_PARA_VENCER; // Retorna verdadeiro se a contagem for suficiente
    }
    return 0;
}

// Função utilitária para limpar o buffer de entrada do teclado (stdin).
// Necessária para evitar que o "Enter" de um scanf afete a próxima leitura (ex: um getchar).
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}