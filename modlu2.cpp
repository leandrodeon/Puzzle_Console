#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#define MAX 10

// Declarações de funções
int dimensoes();
int carregar_user();
void ranking();
int menu();
void iniciar_tabuleiro(int matriz[MAX][MAX], int n);
void embaralhar_tabuleiro(int matriz[MAX][MAX], int n);
int mover(int matriz[MAX][MAX], int n, char movimento);
int verificar_vitoria(int matriz[MAX][MAX], int n);
void imprimir_tabuleiro(int matriz[MAX][MAX], int n);
void imprimir_movimento(int matriz[MAX][MAX], int n, char movimento);
void salvar_pontuacao(const char *nome, int pontuacao);

typedef struct {
    char user[50];
    int pontuacao;
} user;

int dimensoes() {
    int nxn;
    printf("\nInforme o tamanho do tabuleiro (N x N)");
    printf("\nDigite um número inteiro entre 3 e 10.");
    printf("\n[N]: ");
    scanf("%d", &nxn);
    printf("\n[OK] Tabuleiro NxN criado! %dx%d", nxn, nxn);
    printf("\nMisturando peças...\n");
    return nxn;
}

int carregar_user(user jogadores[], int max) {
    FILE *cadastro_user = fopen("cadastro.txt", "r");
    if (!cadastro_user) {
        printf("\n[ERRO] Não foi possível abrir o arquivo 'cadastro.txt'.\n");
        return 0;
    }

    int count = 0;
    while (count < max && fscanf(cadastro_user, "%49s %d", jogadores[count].user, &jogadores[count].pontuacao) == 2) {
        count++;
    }

    fclose(cadastro_user);
    return count;
}

void salvar_pontuacao(const char *nome, int pontuacao) {
    user jogadores[MAX];
    int total = carregar_user(jogadores, MAX);
    int i, encontrado = 0;

    for (i = 0; i < total; i++) {
        if (strcmp(jogadores[i].user, nome) == 0) {
            if (pontuacao > jogadores[i].pontuacao) {
                jogadores[i].pontuacao = pontuacao;
            }
            encontrado = 1;
            break;
        }
    }

    if (!encontrado && total < MAX) {
        strncpy(jogadores[total].user, nome, sizeof(jogadores[total].user) - 1);
        jogadores[total].user[sizeof(jogadores[total].user) - 1] = '\0';
        jogadores[total].pontuacao = pontuacao;
        total++;
    }

    FILE *cadastro_user = fopen("cadastro.txt", "w");
    if (!cadastro_user) {
        printf("\n[ERRO] Não foi possível salvar no arquivo 'cadastro.txt'.\n");
        return;
    }

    for (i = 0; i < total; i++) {
        fprintf(cadastro_user, "%s %d\n", jogadores[i].user, jogadores[i].pontuacao);
    }

    fclose(cadastro_user);
}

void ranking() {
    user jogadores[MAX];
    int total = carregar_user(jogadores, MAX);

    if (total == 0) {
        printf("\n[!] Nenhum jogador encontrado no ranking.\n");
        return;
    }

    // Bubble sort
    for (int a = 0; a < total - 1; a++) {
        for (int b = a + 1; b < total; b++) {
            if (jogadores[b].pontuacao > jogadores[a].pontuacao) {
                user temp = jogadores[a];
                jogadores[a] = jogadores[b];
                jogadores[b] = temp;
            }
        }
    }

    printf("\n===== RANKING =====\n");
    for (int i = 0; i < total; i++) {
        printf("%d - %s: %d pontos\n", i + 1, jogadores[i].user, jogadores[i].pontuacao);
    }
}

// // Função para imprimir o tabuleiro
// void imprimir_tabuleiro(int matriz[MAX][MAX], int n) {
//     printf("\n===== TABULEIRO %dx%d =====\n", n, n);
//     for (int i = 0; i < n; i++) {
//         for (int j = 0; j < n; j++) {
//             if (matriz[i][j] == 0)
//                 printf("   ");
//             else
//                 printf("%3d", matriz[i][j]);
//         }
//         printf("\n");
//     }
//     printf("===========================\n");
// }

// Função para imprimir o tabuleiro (layout igual ao ImprimirMatriz)
void imprimir_tabuleiro(int matriz[MAX][MAX], int n) {
    int i, j;

    printf("\n");

    // Topo
    printf("\t+");
    for (j = 0; j < n; j++) {
        printf("----");
        if (j < n - 1) {
            printf("+");
        }
    }
    printf("+\n");

    // Linhas da matriz
    for (i = 0; i < n; i++) {
        printf("\t|");
        for (j = 0; j < n; j++) {
            if (matriz[i][j] == 0) {
                // Espaço vazio no lugar do 0
                printf("    |");
            } else {
                printf(" %2d |", matriz[i][j]);
            }
        }
        printf("\n");

        // Linha de separação / rodapé
        printf("\t+");
        for (j = 0; j < n; j++) {
            printf("----");
            if (j < n - 1) {
                printf("+");
            }
        }
        printf("+\n");
    }

    printf("\n");
}


// Função que imprime o movimento feito pelo jogador
void imprimir_movimento(int matriz[MAX][MAX], int n, char movimento) {
    system("cls || clear");
    printf("\n===== MOVIMENTO FEITO =====\n");

    switch (movimento) {
        case 'w': case 'W': printf("Movimento: CIMA (W)\n"); break;
        case 's': case 'S': printf("Movimento: BAIXO (S)\n"); break;
        case 'a': case 'A': printf("Movimento: ESQUERDA (A)\n"); break;
        case 'd': case 'D': printf("Movimento: DIREITA (D)\n"); break;
        default: printf("Movimento: inválido (%c)\n", movimento);
    }

    printf("===========================\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matriz[i][j] == 0)
                printf("   ");
            else
                printf("%3d", matriz[i][j]);
        }
        printf("\n");
    }
    printf("===========================\n");
}

// Inicializa o tabuleiro ordenado
void iniciar_tabuleiro(int matriz[MAX][MAX], int n) {
    int valor = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matriz[i][j] = valor++;
        }
    }
    matriz[n - 1][n - 1] = 0;
}

// Move o bloco conforme o input
int mover(int matriz[MAX][MAX], int n, char movimento) {
    int linhaVazia, colunaVazia;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (matriz[i][j] == 0) {
                linhaVazia = i;
                colunaVazia = j;
            }

    int novaLinha = linhaVazia, novaColuna = colunaVazia;

    switch (movimento) {
        case 'w': case 'W': novaLinha++; break;
        case 's': case 'S': novaLinha--; break;
        case 'a': case 'A': novaColuna++; break;
        case 'd': case 'D': novaColuna--; break;
        default: return 0;
    }

    if (novaLinha < 0 || novaLinha >= n || novaColuna < 0 || novaColuna >= n)
        return 0;

    int temp = matriz[novaLinha][novaColuna];
    matriz[novaLinha][novaColuna] = 0;
    matriz[linhaVazia][colunaVazia] = temp;

    return 1;
}

// Embaralha o tabuleiro
void embaralhar_tabuleiro(int matriz[MAX][MAX], int n) {
    for (int i = 0; i < n * n * 5; i++) {
        char movs[] = {'w', 'a', 's', 'd'};
        char mov = movs[rand() % 4];
        mover(matriz, n, mov);
    }
}

// Verifica se o tabuleiro está na ordem correta
int verificar_vitoria(int matriz[MAX][MAX], int n) {
    int valor = 1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == n - 1 && j == n - 1)
                return 1;
            if (matriz[i][j] != valor)
                return 0;
            valor++;
        }
    }
    return 1;
}

int menu() {
    int opcao, n;

    do {
        printf("\n===============================================");
        printf("\n                 SLIDE PUZZLE                   ");
        printf("\n===============================================");
        printf("\n[1] Jogar                                      ");
        printf("\n[2] Ver Ranking                                ");
        printf("\n[3] Sair                                       ");
        printf("\n===============================================\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                n = dimensoes();
                int matriz[MAX][MAX];
                iniciar_tabuleiro(matriz, n);
                embaralhar_tabuleiro(matriz, n);

                char movimento;
                int movimentos = 0;
                char nome[50];

                printf("\nDigite seu nome (sem espaços): ");
                scanf("%49s", nome);

                imprimir_tabuleiro(matriz, n);

                do {
                    printf("\nDigite seu movimento (W/A/S/D) ou X para sair: ");
                    scanf(" %c", &movimento);
                    if (movimento == 'x' || movimento == 'X') {
                        printf("\nSaindo da partida...\n");
                        break;
                    }

                    if (mover(matriz, n, movimento)) {
                        movimentos++;
                        imprimir_movimento(matriz, n, movimento);
                    } else {
                        printf("\nMovimento inválido!\n");
                    }

                } while (!verificar_vitoria(matriz, n));

                if (verificar_vitoria(matriz, n)) {
                    int pontuacao = 1000 - movimentos;
                    if (pontuacao < 0) pontuacao = 0;
                    printf("\nParabéns! Você completou o puzzle!\n");
                    printf("Movimentos: %d | Pontuação: %d\n", movimentos, pontuacao);
                    salvar_pontuacao(nome, pontuacao);
                }
                break;
            }

            case 2:
                ranking();
                break;

            case 3:
                printf("Saindo...\n");
                return 0;

            default:
                printf("\nOpção inválida. Pressione ENTER para continuar");
                while (getchar() != '\n');
                getchar();
        }
    } while (opcao != 3);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    srand(time(NULL)); // embaralhamento aleatório
    menu();
    return 0;
}
