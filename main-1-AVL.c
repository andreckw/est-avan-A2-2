#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Usuario {
    char nome[100];
    int id;
    char email[100];
    int altura;
    struct Usuario* esq;
    struct Usuario* dir;
} Usuario;

int altura(Usuario* no) {
    return (no == NULL) ? 0 : no->altura;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

Usuario* rotacaoDireita(Usuario* y) {
    Usuario* x = y->esq;
    Usuario* T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = max(altura(y->esq), altura(y->dir)) + 1;
    x->altura = max(altura(x->esq), altura(x->dir)) + 1;

    return x;
}

Usuario* rotacaoEsquerda(Usuario* x) {
    Usuario* y = x->dir;
    Usuario* T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = max(altura(x->esq), altura(x->dir)) + 1;
    y->altura = max(altura(y->esq), altura(y->dir)) + 1;

    return y;
}

int fatorBalanceamento(Usuario* no) {
    return (no == NULL) ? 0 : altura(no->esq) - altura(no->dir);
}

Usuario* criarUsuario(char* nome, int id, char* email) {
    Usuario* novo = (Usuario*)malloc(sizeof(Usuario));
    strcpy(novo->nome, nome);
    novo->id = id;
    strcpy(novo->email, email);
    novo->esq = novo->dir = NULL;
    novo->altura = 1;
    return novo;
}

Usuario* inserirUsuario(Usuario* raiz, char* nome, int id, char* email) {
    if (raiz == NULL) return criarUsuario(nome, id, email);

    if (strcmp(nome, raiz->nome) < 0)
        raiz->esq = inserirUsuario(raiz->esq, nome, id, email);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->dir = inserirUsuario(raiz->dir, nome, id, email);
    else
        return raiz;

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));

    int balance = fatorBalanceamento(raiz);

    if (balance > 1 && strcmp(nome, raiz->esq->nome) < 0)
        return rotacaoDireita(raiz);
    if (balance < -1 && strcmp(nome, raiz->dir->nome) > 0)
        return rotacaoEsquerda(raiz);
    if (balance > 1 && strcmp(nome, raiz->esq->nome) > 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }
    if (balance < -1 && strcmp(nome, raiz->dir->nome) < 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

Usuario* buscarUsuario(Usuario* raiz, char* nome) {
    if (raiz == NULL || strcmp(nome, raiz->nome) == 0)
        return raiz;
    if (strcmp(nome, raiz->nome) < 0)
        return buscarUsuario(raiz->esq, nome);
    else
        return buscarUsuario(raiz->dir, nome);
}

Usuario* menorUsuario(Usuario* raiz) {
    Usuario* atual = raiz;
    while (atual && atual->esq != NULL)
        atual = atual->esq;
    return atual;
}

Usuario* removerUsuario(Usuario* raiz, char* nome) {
    if (raiz == NULL) return raiz;

    if (strcmp(nome, raiz->nome) < 0)
        raiz->esq = removerUsuario(raiz->esq, nome);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->dir = removerUsuario(raiz->dir, nome);
    else {
        if (raiz->esq == NULL || raiz->dir == NULL) {
            Usuario* temp = (raiz->esq) ? raiz->esq : raiz->dir;
            if (temp == NULL) {
                temp = raiz;
                raiz = NULL;
            } else {
                *raiz = *temp;
            }
            free(temp);
        } else {
            Usuario* temp = menorUsuario(raiz->dir);
            strcpy(raiz->nome, temp->nome);
            raiz->id = temp->id;
            strcpy(raiz->email, temp->email);
            raiz->dir = removerUsuario(raiz->dir, temp->nome);
        }
    }

    if (raiz == NULL) return raiz;

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));

    int balance = fatorBalanceamento(raiz);

    if (balance > 1 && fatorBalanceamento(raiz->esq) >= 0)
        return rotacaoDireita(raiz);
    if (balance > 1 && fatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
    }
    if (balance < -1 && fatorBalanceamento(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz);
    if (balance < -1 && fatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
    }

    return raiz;
}

Usuario* buscaid(Usuario* raiz, int id) {
    if (raiz == NULL)
        return NULL;

    if (raiz->id == id)
        return raiz;

    Usuario* esquerda = buscaid(raiz->esq, id);
    if (esquerda != NULL)
        return esquerda;

    return buscaid(raiz->dir, id);
}

void listarUsuarios(Usuario* raiz) {
    if (raiz != NULL) {
        listarUsuarios(raiz->esq);
        printf("Nome: %s | ID: %d | Email: %s\n", raiz->nome, raiz->id, raiz->email);
        listarUsuarios(raiz->dir);
    }
}

int main() {
    Usuario* raiz = NULL;
    int opcao, id;
    char nome[100], email[100];

    do {
        printf("1. Cadastrar Usuario\n");
        printf("2. Remover Usuario\n");
        printf("3. Buscar Usuario\n");
        printf("4. Listar Usuarios em ordem alfabetica\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Nome: ");
                scanf("%s", nome);
                printf("ID: ");
                scanf("%d", &id);

                if (buscaid(raiz, id) != NULL) {
                    printf("Ja tem um usuario com esse id\n");
                    break;
                }
                
                printf("Email: ");
                scanf("%s", email);
                raiz = inserirUsuario(raiz, nome, id, email);
                break;

            case 2:
                printf("Nome a remover: ");
                scanf("%s", nome);
                raiz = removerUsuario(raiz, nome);
                break;

            case 3:
                printf("Nome a buscar: ");
                scanf("%s", nome);
                Usuario* encontrado = buscarUsuario(raiz, nome);
                if (encontrado)
                    printf("Nome: %s | ID: %d | Email: %s\n", encontrado->nome, encontrado->id, encontrado->email);
                else
                    printf("Usuario nao encontrado\n");
                break;

            case 4:
                listarUsuarios(raiz);
                break;

            case 0:
                printf("Saindo\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return 0;
}
