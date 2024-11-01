#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int matricula;
    char nome[50];
    char data_entrada[11];
    char curso[50];
} Aluno;

typedef struct No {
    Aluno aluno;
    int altura;
    struct No *esquerda;
    struct No *direita;
} No;

// Funções de árvore AVL
int altura(No *no) {
    return no ? no->altura : -1;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

void atualizar_altura(No *no) {
    no->altura = max(altura(no->esquerda), altura(no->direita)) + 1;
}

No* rotacao_esquerda(No *y) {
    No *x = y->direita;
    y->direita = x->esquerda;
    x->esquerda = y;
    atualizar_altura(y);
    atualizar_altura(x);
    return x;
}

No* rotacao_direita(No *y) {
    No *x = y->esquerda;
    y->esquerda = x->direita;
    x->direita = y;
    atualizar_altura(y);
    atualizar_altura(x);
    return x;
}

No* rotacao_esquerda_direita(No *no) {
    no->esquerda = rotacao_esquerda(no->esquerda);
    return rotacao_direita(no);
}

No* rotacao_direita_esquerda(No *no) {
    no->direita = rotacao_direita(no->direita);
    return rotacao_esquerda(no);
}

int fator_balanceamento(No *no) {
    return altura(no->esquerda) - altura(no->direita);
}

No* balancear(No *no) {
    int balance = fator_balanceamento(no);
    if (balance > 1) {
        if (fator_balanceamento(no->esquerda) < 0)
            no = rotacao_esquerda_direita(no);
        else
            no = rotacao_direita(no);
    } else if (balance < -1) {
        if (fator_balanceamento(no->direita) > 0)
            no = rotacao_direita_esquerda(no);
        else
            no = rotacao_esquerda(no);
    } else {
        atualizar_altura(no);
    }
    return no;
}

No* inserir(No *raiz, Aluno aluno) {
    if (!raiz) {
        No *novo = (No*)malloc(sizeof(No));
        novo->aluno = aluno;
        novo->altura = 0;
        novo->esquerda = novo->direita = NULL;
        return novo;
    }

    if (aluno.matricula < raiz->aluno.matricula)
        raiz->esquerda = inserir(raiz->esquerda, aluno);
    else if (aluno.matricula > raiz->aluno.matricula)
        raiz->direita = inserir(raiz->direita, aluno);
    else
        return raiz;  // Evitar duplicatas

    return balancear(raiz);
}

No* encontrar_minimo(No *no) {
    while (no->esquerda) no = no->esquerda;
    return no;
}

No* remover(No *raiz, int matricula) {
    if (!raiz) return NULL;

    if (matricula < raiz->aluno.matricula)
        raiz->esquerda = remover(raiz->esquerda, matricula);
    else if (matricula > raiz->aluno.matricula)
        raiz->direita = remover(raiz->direita, matricula);
    else {
        if (!raiz->esquerda || !raiz->direita) {
            No *temp = raiz->esquerda ? raiz->esquerda : raiz->direita;
            free(raiz);
            return temp;
        } else {
            No *temp = encontrar_minimo(raiz->direita);
            raiz->aluno = temp->aluno;
            raiz->direita = remover(raiz->direita, temp->aluno.matricula);
        }
    }

    return balancear(raiz);
}

Aluno* buscar(No *raiz, int matricula) {
    if (!raiz) return NULL;
    if (matricula < raiz->aluno.matricula)
        return buscar(raiz->esquerda, matricula);
    else if (matricula > raiz->aluno.matricula)
        return buscar(raiz->direita, matricula);
    else
        return &raiz->aluno;
}

void imprimir_arvore(No *raiz, int nivel) {
    if (!raiz) return;
    imprimir_arvore(raiz->direita, nivel + 1);
    for (int i = 0; i < nivel; i++) printf("   ");
    printf("%d\n", raiz->aluno.matricula);
    imprimir_arvore(raiz->esquerda, nivel + 1);
}

// Função para exibir o menu
void exibir_menu() {
    printf("\n--- Menu ---\n");
    printf("1. Inserir aluno\n");
    printf("2. Buscar aluno\n");
    printf("3. Imprimir arvore\n");
    printf("4. Remover aluno\n");
    printf("5. Sair\n");
    printf("Escolha uma opcao: ");
}

int main() {
    No *raiz = NULL;
    int opcao, matricula;
    Aluno novo_aluno;
    Aluno *aluno_encontrado;

    do {
        exibir_menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("\nInserir novo aluno:\n");
                printf("Matricula: ");
                scanf("%d", &novo_aluno.matricula);
                getchar(); // Limpar o buffer de entrada
                printf("Nome: ");
                fgets(novo_aluno.nome, 50, stdin);
                novo_aluno.nome[strcspn(novo_aluno.nome, "\n")] = '\0'; // Remover newline
                printf("Data de entrada (YYYY-MM-DD): ");
                scanf("%s", novo_aluno.data_entrada);
                printf("Curso: ");
                getchar(); // Limpar o buffer de entrada
                fgets(novo_aluno.curso, 50, stdin);
                novo_aluno.curso[strcspn(novo_aluno.curso, "\n")] = '\0'; // Remover newline

                raiz = inserir(raiz, novo_aluno);
                printf("Aluno inserido com sucesso!\n");
                break;

            case 2:
                printf("\nBuscar aluno:\n");
                printf("Digite a matricula: ");
                scanf("%d", &matricula);
                aluno_encontrado = buscar(raiz, matricula);
                if (aluno_encontrado) {
                    printf("Encontrado: %s, Matricula: %d, Data de entrada: %s, Curso: %s\n",
                           aluno_encontrado->nome, aluno_encontrado->matricula,
                           aluno_encontrado->data_entrada, aluno_encontrado->curso);
                } else {
                    printf("Aluno nao encontrado.\n");
                }
                break;

            case 3:
                printf("\nArvore AVL:\n");
                imprimir_arvore(raiz, 0);
                break;

            case 4:
                printf("\nRemover aluno:\n");
                printf("Digite a matricula: ");
                scanf("%d", &matricula);
                raiz = remover(raiz, matricula);
                printf("Aluno removido (se encontrado).\n");
                break;

            case 5:
                printf("Saindo do programa...\n");
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 5);

    return 0;
}