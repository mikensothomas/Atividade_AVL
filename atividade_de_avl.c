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

// Função para obter a altura do nó
int altura(No *no) {
    return no ? no->altura : -1;
}

// Função para atualizar a altura do nó
int max(int a, int b) {
    return (a > b) ? a : b;
}

void atualizar_altura(No *no) {
    no->altura = max(altura(no->esquerda), altura(no->direita)) + 1;
}

// Rotação simples à esquerda
No* rotacao_esquerda(No *y) {
    No *x = y->direita;
    y->direita = x->esquerda;
    x->esquerda = y;
    atualizar_altura(y);
    atualizar_altura(x);
    return x;
}

// Rotação simples à direita
No* rotacao_direita(No *y) {
    No *x = y->esquerda;
    y->esquerda = x->direita;
    x->direita = y;
    atualizar_altura(y);
    atualizar_altura(x);
    return x;
}

// Rotação dupla à esquerda e direita
No* rotacao_esquerda_direita(No *no) {
    no->esquerda = rotacao_esquerda(no->esquerda);
    return rotacao_direita(no);
}

No* rotacao_direita_esquerda(No *no) {
    no->direita = rotacao_direita(no->direita);
    return rotacao_esquerda(no);
}

// Função de balanceamento
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

// Função de inserção de aluno
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

// Função para encontrar o menor nó na subárvore direita
No* encontrar_minimo(No *no) {
    while (no->esquerda) no = no->esquerda;
    return no;
}

// Função de remoção de aluno
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

// Função de busca de aluno
Aluno* buscar(No *raiz, int matricula) {
    if (!raiz) return NULL;
    if (matricula < raiz->aluno.matricula)
        return buscar(raiz->esquerda, matricula);
    else if (matricula > raiz->aluno.matricula)
        return buscar(raiz->direita, matricula);
    else
        return &raiz->aluno;
}

// Função de impressão da árvore AVL
void imprimir_arvore(No *raiz, int nivel) {
    if (!raiz) return;
    imprimir_arvore(raiz->direita, nivel + 1);
    for (int i = 0; i < nivel; i++) printf("   ");
    printf("%d\n", raiz->aluno.matricula);
    imprimir_arvore(raiz->esquerda, nivel + 1);
}

// Função principal para teste
int main() {
    No *raiz = NULL;

    Aluno aluno1 = {1, "Alice", "2022-02-10", "Ciencia da Computacao"};
    Aluno aluno2 = {2, "Bob", "2021-08-15", "Engenharia"};
    Aluno aluno3 = {3, "João", "2023-01-12", "Matematica"};
    Aluno aluno4 = {7, "Mateus", "2023-03-12", "Quimica"};
    Aluno aluno5 = {9, "Pierre", "2023-04-12", "Biologia"};
    Aluno aluno6 = {10, "Lucas", "2023-05-12", "Letra"};
    Aluno aluno7 = {20, "Jean", "2023-06-12", "Lingua"};

    raiz = inserir(raiz, aluno1);
    raiz = inserir(raiz, aluno2);
    raiz = inserir(raiz, aluno3);
    raiz = inserir(raiz, aluno4);
    raiz = inserir(raiz, aluno5);
    raiz = inserir(raiz, aluno6);
    raiz = inserir(raiz, aluno7);

    printf("Arvore AVL:\n");
    imprimir_arvore(raiz, 0);

    printf("\nBuscando aluno com matricula:\n");
    Aluno *aluno = buscar(raiz, 2);
    if (aluno) {
        printf("Encontrado: %s %i\n", aluno->nome, aluno->matricula);
    } else {
        printf("Aluno nao cadastrado.\n");
    }

    printf("\nRemovendo aluno com matricula 2.\n");
    raiz = remover(raiz, 2);

    printf("Arvore AVL apos remocao:\n");
    imprimir_arvore(raiz, 0);

    return 0;
}