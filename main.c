# include <stdio.h>
# include <string.h>
# include <stdlib.h>

// Tipo base dos elementos da árvore
typedef struct elementos {
    int rgm;
    char nome[100];    
} t_elemento;

// Criação de nó e ponteiros esquerda e direita
typedef struct no {
    struct no * esq;
    t_elemento dado;
    struct no * dir;
} t_no;

typedef t_no * t_arvore; // Alias de t_no chamado t_arvore (usa "t_arvore" ao invés de "t_no *")

// Função que será usada em compara()
int compara(t_elemento a, t_elemento b) {
    return (a.rgm - b.rgm);
}

// Função para realizar a busca por um RGM específico
t_no * busca(t_arvore tree, t_elemento dado) {
    if (tree == NULL) {
        return NULL;
    }
    if (compara(tree->dado, dado)==0) {
        return tree;
    }
    if (compara(tree->dado, dado) > 0) {
        return busca(tree -> esq, dado);
    } else {
        return busca(tree -> dir, dado);
    }
}

// Função para exibir em PréOrdem (UED)
void exibirPreOrdem(t_arvore tree) {
    if (tree != NULL) {
        printf("%s", tree->dado.nome);
        exibirPreOrdem(tree -> esq);
        exibirPreOrdem(tree -> dir);
    }
}

// Função para exibir InOrdem (EUD)
void exibirInOrdem(t_arvore tree) {
    if (tree != NULL) {
        exibirInOrdem(tree -> esq);
        printf("%s", tree -> dado.nome);
        exibirInOrdem(tree -> dir);
    }
}

// Função para exibir em PósOrdem (EDU)
void exibirPosOrdem (t_arvore tree) {
    if (tree != NULL) {
        exibirPosOrdem(tree -> esq);
        exibirPosOrdem(tree -> dir);
        printf("%s", tree -> dado.nome);
    }
}

int main () {

    
    return 0;
}