# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <locale.h>

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

// Cria um novo nó vazio
t_no * criar() {
    t_no * no = (t_no*)malloc(sizeof(t_no)); // Alocação Dinâmica
    if (no) {
        no -> esq = no -> dir = NULL;   // Inicializa os nós nulos
    }
    return no;
}

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

int inserir(t_arvore * tree, t_elemento item) {
    int ok;
    // se a raiz for nula, entao insere na raiz
    if (*tree == NULL) {
        *tree = criar();
        if (*tree == NULL) {
            return 0;       // se mesmo assim estiver nula, retorna 0 (erro)
        }
        (*tree) -> dado = item;
        return 1;
    }
    // Compara o item com o dado do nó atual
    int comp = compara((*tree)->dado, item);
    if (comp < 0) {
        ok = inserir (&((*tree) -> esq), item);         // se menor que o pai, vai pra esquerda
    } else if (comp > 0) {
        ok = inserir (&((*tree) -> dir), item);         // se maior que o pai, vai pra direita
    } else {
        ok = 0;                                         // igual ao dado atual, não insere nada
    }
    return ok;
}

// Função para buscar e setar o pai
t_no * buscaSetPai(t_arvore tree, t_elemento dado, t_no ** pai) {
    if (tree == NULL) {
        *pai = NULL;        // se árvore vazia, pai nulo
        return NULL;
    }
    
    // Compara o nó atual com o dado nó
    if (compara(tree -> dado, dado) == 0) {
        return tree;        // encontrou, é retornado
    }

    if (compara(tree -> dado, dado) > 0) {
        // Vai para a subárvore esquerda
        *pai = tree;
        return buscaSetPai(tree -> esq, dado, pai);     // pai na esquerda
    } else {
        // Vai para a subárvore direita
        *pai = tree;
        return buscaSetPai(tree -> dir, dado, pai);     // pai na direita
    }    
}

// Função para remover nó por RGM
int remover (t_arvore *tree, t_elemento item) {
    t_no *no, // no aponta para o no a ser removido
        *pai, // pai aponta para o pai do no
        *sub, // sub aponta que ira substituir o no no
        *paiSuce, // pai do no sucessor
        *suce; // sucessor do no no

    no = *tree; pai=NULL;
    no = buscaSetPai(*tree, item, &pai);        // Busca o nó a ser removido, e seta o seu pai

    if (no==NULL) {
        return 0; // Não existe na arvore -> não consegue remover   
    }

    // CASO QUE O NÓ TEM UM FILHO NO MÁXIMO
    if (no->esq == NULL)
        sub = no->dir;          // Se não tá na esquerda -> tá na direita
    else {
        if (no->dir == NULL) {
            sub = no->esq;      // Se não tá na direita -> tá na esquerda
        } else {                // CASO QUE O NÓ TEM DOIS FILHOS
            paiSuce = no;
            sub = no -> dir;
            suce = sub -> esq;       // suce é sempre o filho esquerdo de sub
                while (suce != NULL) {      // Checa até achar um valor
                    paiSuce = sub;
                    sub = suce;
                    suce = sub->esq;
                }
               // neste ponto, sub eh o sucessor em ordem de no
               if (paiSuce != no) {
                   //  no não é o pai de sub, e sub == paiSuce->esq
                   paiSuce->esq = sub->dir;     // remove o no sub de sua atual posicao e o substitui pelo filho direito de sub
                   sub->dir = no->dir;          // sub ocupa o lugar de no
               }
               sub->esq = no->esq;              // define o filho esquerdo de sub de modo que sub ocupe o lugar de no
        }
           
    }
    // insere sub na posicao ocupada anteriormente por no
    if (pai == NULL) {      // Se nó é raiz, não tem pai
        *tree = sub;
    }
    else {
        // Verifica se o nó é filho da esquerda ou direita
        if (no == pai->esq) {
            pai->esq = sub;
        } else {
            pai->dir = sub;
        }
    }
    free(no); // Libera a memória alocada no nó
    return 1; // verdadeiro, conseguiu remover
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
        printf("%d | %s", tree -> dado.rgm, tree -> dado.nome);
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

    setlocale(LC_ALL, "Portuguese");


    return 0;
}