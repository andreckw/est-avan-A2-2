#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { RED, BLACK } Color;


typedef struct Produto {
	int codigo;
	char nome[50];
	int qtd_estoque;
	float preco;
	Color cor;
	struct Produto *esq, *dir, *pai;
} Produto;

Produto* rotacaoEsquerda(Produto* raiz, Produto* x) {
    Produto* y = x->dir;
    x->dir = y->esq;
    if (y->esq != NULL) {
    	y->esq->pai = x;
	}
	
    y->pai = x->pai;
    
    if (x->pai == NULL) {
    	raiz = y;	
	} else if (x == x->pai->esq) {
		x->pai->esq = y;	
	} else {
		x->pai->dir = y;	
	}
	
    y->esq = x;
    x->pai = y;
    return raiz;
}


Produto* rotacaoDireita(Produto* raiz, Produto* y) {
    Produto* x = y->esq;
    y->esq = x->dir;
    if (x->dir != NULL) {
    	x->dir->pai = y;	
	}
	
    x->pai = y->pai;
    
    if (y->pai == NULL) {
    	raiz = x;	
	} else if (y == y->pai->dir) {
		y->pai->dir = x;	
	} else {
		y->pai->esq = x;	
	}
	
    x->dir = y;
    y->pai = x;
    return raiz;
}

Produto* inserirBST(Produto* raiz, Produto* novo) {
    if (raiz == NULL) {
    	return novo;
	}

    if (novo->codigo < raiz->codigo) {
        raiz->esq = inserirBST(raiz->esq, novo);
        raiz->esq->pai = raiz;
    } else if (novo->codigo > raiz->codigo) {
        raiz->dir = inserirBST(raiz->dir, novo);
        raiz->dir->pai = raiz;
    }

    return raiz;
}

Produto* corrigirInsercao(Produto* raiz, Produto* no) {
    Produto* tio;
    while (no != raiz && no->pai->cor == RED) {
        if (no->pai == no->pai->pai->esq) {
            tio = no->pai->pai->dir;
            if (tio != NULL && tio->cor == RED) {
                no->pai->cor = BLACK;
                tio->cor = BLACK;
                no->pai->pai->cor = RED;
                no = no->pai->pai;
            } else {
                if (no == no->pai->dir) {
                    no = no->pai;
                    raiz = rotacaoEsquerda(raiz, no);
                }
                no->pai->cor = BLACK;
                no->pai->pai->cor = RED;
                raiz = rotacaoDireita(raiz, no->pai->pai);
            }
        } else {
            tio = no->pai->pai->esq;
            if (tio != NULL && tio->cor == RED) {
                no->pai->cor = BLACK;
                tio->cor = BLACK;
                no->pai->pai->cor = RED;
                no = no->pai->pai;
            } else {
                if (no == no->pai->esq) {
                    no = no->pai;
                    raiz = rotacaoDireita(raiz, no);
                }
                no->pai->cor = BLACK;
                no->pai->pai->cor = RED;
                raiz = rotacaoEsquerda(raiz, no->pai->pai);
            }
        }
    }
    raiz->cor = BLACK;
    return raiz;
}

Produto* inserir(Produto* raiz, int codigo, char* nome, int qtd_estoque, float preco) {
	
    Produto* novo = (Produto*)malloc(sizeof(Produto));
    novo->codigo = codigo;
    novo->preco = preco;
    novo->qtd_estoque = qtd_estoque;
    strcpy(novo->nome, nome);
    novo->cor = RED;
    novo->esq = NULL;
	novo->dir = NULL;
	novo->pai = NULL;
    
    raiz = inserirBST(raiz, novo);
    
    return corrigirInsercao(raiz, novo);
}


Produto* buscar(Produto* raiz, int codigo) {
    if (raiz == NULL || raiz->codigo == codigo) {
    	return raiz;	
	}
	
    if (codigo < raiz->codigo) {
    	return buscar(raiz->esq, codigo);	
	} else {
		return buscar(raiz->dir, codigo);	
	}
}

void transplantar(Produto** raiz, Produto* u, Produto* v) {
    if (u->pai == NULL) {
    	*raiz = v;	
	} else if (u == u->pai->esq) {
    	u->pai->esq = v;	
	} else {
    	u->pai->dir = v;
	}

    if (v != NULL) {
    	v->pai = u->pai;	
	}
}

Produto* minimo(Produto* no) {
    while (no->esq != NULL) {
    	no = no->esq;	
	}
    return no;
}

Produto* corrigirRemocao(Produto* raiz, Produto* x) {
    while (x != raiz && (x == NULL || x->cor == BLACK)) {
        if (x == x->pai->esq) {
            Produto* w = x->pai->dir;
            
            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                raiz = rotacaoEsquerda(raiz, x->pai);
                w = x->pai->dir;
            }
            
            if ((w->esq == NULL || w->esq->cor == BLACK) &&
                (w->dir == NULL || w->dir->cor == BLACK)) {
                w->cor = RED;
                x = x->pai;
            } else {
            	
                if (w->dir == NULL || w->dir->cor == BLACK) {
                    if (w->esq != NULL && w->esq->cor == RED) {
                    	w->esq->cor = BLACK;	
					}
					
                    w->cor = RED;
                    raiz = rotacaoDireita(raiz, w);
                    w = x->pai->dir;
                }


                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                if (w->dir != NULL && w->dir->cor == RED) {
                	w->dir->cor = BLACK;
				}
				
                raiz = rotacaoEsquerda(raiz, x->pai);
                x = raiz;
            }
        } else {
            Produto* w = x->pai->esq;

            if (w->cor == RED) {
                w->cor = BLACK;
                x->pai->cor = RED;
                raiz = rotacaoDireita(raiz, x->pai);
                w = x->pai->esq;
            }

            if ((w->esq == NULL || w->esq->cor == BLACK) &&
                (w->dir == NULL || w->dir->cor == BLACK)) {
                w->cor = RED;
                x = x->pai;
            } else {
                if (w->esq == NULL || w->esq->cor == BLACK) {
                    if (w->dir != NULL && w->dir->cor == RED) w->dir->cor = BLACK;
                    w->cor = RED;
                    raiz = rotacaoEsquerda(raiz, w);
                    w = x->pai->esq;
                }

                w->cor = x->pai->cor;
                x->pai->cor = BLACK;
                if (w->esq != NULL && w->esq->cor == RED) w->esq->cor = BLACK;
                raiz = rotacaoDireita(raiz, x->pai);
                x = raiz;
            }
        }
    }

    if (x != NULL) {
    	x->cor = BLACK;	
	}

    return raiz;
}

Produto* remover(Produto* raiz, int codigo) {
    Produto* z = buscar(raiz, codigo);
    if (z == NULL) {
    	return raiz;	
	}

    Produto *y = z;
    Produto *x;
    Color cor_original = y->cor;

    if (z->esq == NULL) {
        x = z->dir;
        transplantar(&raiz, z, z->dir);
    } else if (z->dir == NULL) {
        x = z->esq;
        transplantar(&raiz, z, z->esq);
    } else {
        y = minimo(z->dir);
        cor_original = y->cor;
        x = y->dir;

        if (y->pai == z) {
            if (x != NULL) x->pai = y;
        } else {
            transplantar(&raiz, y, y->dir);
            y->dir = z->dir;
            y->dir->pai = y;
        }

        transplantar(&raiz, z, y);
        y->esq = z->esq;
        y->esq->pai = y;
        y->cor = z->cor;
    }

    free(z);

    if (cor_original == BLACK) {
        raiz = corrigirRemocao(raiz, x);
    }

    return raiz;
}

Produto* cadastrarProduto(Produto *raiz) {
	int codigo;
	char nome[50];
	int qtd_estoque = 0;
	float preco = 0.0;
	
	printf("Digite o codigo do produto: ");
	scanf("%d", &codigo);
	
	printf("Digite nome do produto: ");
	scanf("%s", nome);
	
	printf("Digite a quantidade de estoque do produto: ");
	scanf("%d", &qtd_estoque);
	
	printf("Digite o valor do produto: ");
	scanf("%f", &preco);
	
	return inserir(raiz, codigo, nome, qtd_estoque, preco);
}

void listarProdutos(Produto* raiz) {
	if (raiz == NULL) {
    	return;	
	}
	
    listarProdutos(raiz->esq);
    
    char cor = 'R';
    
    if (raiz->cor == BLACK) {
    	cor = 'B';
	}
    
    printf("(%d) %s\n", raiz->codigo, raiz->nome);
    printf("->valor: %.2f\n", raiz->preco);
    printf("->quantidade estoque: %d\n", raiz->qtd_estoque);
    printf("->cor: %c\n", cor);
    
    listarProdutos(raiz->dir);
}

void buscarProduto(Produto* raiz) {
	int codigo = 0;
	
	printf("Digite o código do produto: ");
	scanf("%d", &codigo);
	
	Produto *achado = buscar(raiz, codigo);
	
	if (achado == NULL) {
		printf("Não foi possivel achar esse código de produto\n");
	} else {
		char cor = 'R';
    
	    if (achado->cor == BLACK) {
	    	cor = 'B';
		}
	    
	    printf("(%d) %s\n", achado->codigo, achado->nome);
	    printf("->valor: %.2f\n", achado->preco);
	    printf("->quantidade estoque: %d\n", achado->qtd_estoque);
	    printf("->cor: %c\n", cor);
	}
	
}

Produto *removerProduto(Produto *raiz) {
	int codigo = 0;
	
	printf("Digite o código do produto: ");
	scanf("%d", &codigo);
	
	return remover(raiz, codigo);
}

int main() {
    Produto* raiz = NULL;
    int escolha = 0;
    
    while (1) {
    	printf("Escolha uma das opções abaixo (ARB)\n");
    	printf("[1] Cadastrar produto\n");
    	printf("[2] Listar produtos\n");
    	printf("[3] Buscar produto\n");
    	printf("[4] Remover produto\n");
    	scanf("%d", &escolha);
    	
    	printf("*-----------*\n");
    	switch (escolha) {
    		case 1:
    			raiz = cadastrarProduto(raiz);
    			break;
    		case 2:
    			listarProdutos(raiz);
    			break;
    		case 3:
    			buscarProduto(raiz);
    			break;
    		case 4:
    			raiz = removerProduto(raiz);
    			break;
		}
		printf("*-----------*\n\n");
	}

    return 0;
}
