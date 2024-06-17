# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <string.h>
# include <conio.h>
# include <windows.h>

// Cores do terminal
#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define BLUE "\e[0;34m"
#define CRESET "\e[0m"

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
    if (comp > 0) {
        ok = inserir (&((*tree) -> esq), item);         // se menor que o pai, vai pra esquerda
    } else if (comp < 0) {
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

void esvaziar(t_arvore *tree) {
    if (*tree == NULL) {
        return;     // Condição de parada (caso nula)
    }
    esvaziar(&(*tree)->esq);    // Esvazia esquerda
    esvaziar(&(*tree)->dir);    // Esvazia direita
    free(*tree);                // Libera memória da árvora
    *tree = NULL;               // Seta como nula novamente
}

// Função para exibir em PréOrdem (UED)
void exibirPreOrdem(t_arvore tree) {
    if (tree != NULL) {
        printf("%d | %s\n", tree->dado.rgm, tree->dado.nome);
        exibirPreOrdem(tree -> esq);
        exibirPreOrdem(tree -> dir);
    }
}

// Função para exibir InOrdem (EUD)
void exibirInOrdem(t_arvore tree) {
    if (tree != NULL) {
        exibirInOrdem(tree -> esq);
        printf("%d | %s\n", tree->dado.rgm, tree->dado.nome);
        exibirInOrdem(tree -> dir);
    }
}

// Função para exibir em PósOrdem (EDU)
void exibirPosOrdem (t_arvore tree) {
    if (tree != NULL) {
        exibirPosOrdem(tree -> esq);
        exibirPosOrdem(tree -> dir);
        printf("%d | %s\n", tree->dado.rgm, tree->dado.nome);
    }
}

/* Função para fazer o gotoxy() e função exibirGraficamente() funcionar (Compilador não entende a biblioteca por padrão (?) )
(https://www.quora.com/The-function-gotoxy-is-not-performing-in-my-C-program-when-typed-into-Code-Blocks-What-should-I-do) */
void gotoxy(int coluna, int linha)
{ 
   COORD point;
   point.X = coluna;
   point.Y = linha;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}

// Função para exibir a árvore em pré-ordem
// Sugestão de uso: exibirGraficamente(arvore, 10, 10, 3);
void exibirGraficamente(t_arvore tree, int col, int lin, int desloc) {
    /* Col e Lin são coordenadas da tela onde a árvore irá iniciar, ou seja, a posição da raiz. E desloc representa o deslocamento na tela (em colunas) de um nó em relação ao nó anterior */
    if (tree == NULL) {
        return;         // Condição de parada
    }
    gotoxy(col, lin);
    printf("%d | %s", tree -> dado.rgm, tree-> dado.nome);

    if (tree -> esq != NULL) {
        exibirGraficamente(tree -> esq, col-desloc, lin+2, desloc/2+1);
    }
    if (tree -> dir != NULL) {
        exibirGraficamente(tree -> dir, col+desloc, lin+2, desloc/2+1);
    }
}

// Função para ler arquivo dos Créditos
void lerArquivo(const char *nomeArquivo) {
    FILE *fptr;
    long tamanhoArquivo;
    char *conteudo;

    fptr = fopen(nomeArquivo, "r");    // Abrir um arquivo em modo de leitura
    if (fptr == NULL) {
        printf("Erro ao abrir o arquivo.");     // Checa por erros
        return;
    }

    fseek(fptr, 0, SEEK_END);                   // Mover para o fim do arquivo
    tamanhoArquivo = ftell(fptr);               // Obter o tamanho do arquivo
    rewind(fptr);                               // Voltar ao início do arquivo

    // Alocação dinâmica de memória para armazenar o conteúdo do arquivo
    conteudo = (char *)malloc(sizeof(char) * (tamanhoArquivo + 1));
    if (conteudo == NULL) {
        printf("Erro ao alocar memória.");
        fclose(fptr);
        return;
    }

    fread(conteudo, sizeof(char), tamanhoArquivo, fptr);            // Lê o conteúdo do arquivo para o buffer
    conteudo[tamanhoArquivo] = '\0';                                // procura o "\0" do final do arquivo

    printf("%s", conteudo);                                     // Imprime o conteúdo do arquivo

    free(conteudo);         // Liberação de memória
    fclose(fptr);
}

void menu() {
    printf("\nMENU EDITOR DE ÁRVORE\n");
    printf("1 - Inserir nome e RGM\n");
	printf("2 - Remover um nó\n");
	printf("3 - Pesquisar por RGM\n");
	printf("4 - Esvaziar a árvore\n");
    printf("5 - Exibir árvore\n");
    printf("6 - Créditos\n");
	printf("0 - Fechar programa\n");
}

int main () {

    UINT CPAGE_UTF8 = 65001;            // Código de Internacionalização de caracteres
    SetConsoleOutputCP(CPAGE_UTF8);

    t_arvore arvore = NULL;
    t_elemento elemento;
    int opcao;
    int opExib;

    while (1) {
        menu();
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:     // Opção de inserir
                system("cls");
                printf("Digite um nome: ");
                fflush(stdin);                                              // Limpar o buffer do teclado antes de usar fgets
                fgets(elemento.nome, sizeof(elemento.nome), stdin);         // Lendo para elemento.nome (basicamente mesma coisa do scanf)
                elemento.nome[strcspn(elemento.nome, "\n")] = '\0';         // Remove o caractere de nova linha que fgets adiciona
                
                printf("Digite um RGM: ");
                scanf("%d", &elemento.rgm);         // Ler diretamente para elemento.rgm

                // Checagem
                if (inserir(&arvore, elemento)) {
                    printf(GREEN"Elemento inserido com sucesso!\n"CRESET);
                } else {
                    printf(RED"Erro ao inserir novo elemento.\n"CRESET);
                }
                system("pause");
                system("cls");
                break;
            case 2:     // Opção de remover
                system("cls");
                printf("Digite um RGM para remover o nó: ");
                scanf("%d", &elemento.rgm);
                
                // Checagem
                if (remover(&arvore, elemento)) {
                    printf(GREEN"Elemento removido com sucesso!\n"CRESET);
                } else {
                    printf(RED"Erro ao remover elemento.\n"CRESET);
                }
                system("pause");
				system("cls");
                break;
            case 3:     // Opção de BUSCAR
                system("cls");
                printf("Insira RGM para buscar: ");
                scanf("%d", &elemento.rgm);
                t_no *resultado = busca(arvore, elemento);
                // Checagem
                if (resultado != NULL) {
                    printf(GREEN"Elemento encontrado: %d | %s\n"CRESET, resultado->dado.rgm, resultado->dado.nome);
                } else {
                    printf(RED"Elemento não encontrado.\n"CRESET);
                }
                system("pause");
				system("cls");
                break;
            case 4:     // Função para Esvaziar árvore
                system("cls");
                esvaziar(&arvore);
                printf(GREEN"Árvore apagada com sucesso."CRESET);
                printf("\n\n\n\n");
				system("pause");
				system("cls");
                break;
            case 5:     // Função para exibir a árvore
                system("cls");
                printf(BLUE"Exibir árvore:\n1 - PRÉ-ORDEM\n2 - IN-ORDEM\n3 - PÓS-ORDEM\n4 - Graficamente\n"CRESET);
                printf("Escolha uma opção: ");
                scanf("%d", &opExib);

                switch (opExib) {
                    case 1:     // Exibição Pré-Ordem
                        system("cls");
                        printf(BLUE"Exibição PRÉ-ORDEM:\n"CRESET);
                        exibirPreOrdem(arvore);
                        printf("\n\n\n\n");
						system("pause");
						system("cls");
                        break;
                    case 2:     // Exibição In-Ordem
                        system("cls");
                        printf(BLUE"Exibição IN-ORDEM:\n"CRESET);
                        exibirInOrdem(arvore);
                        printf("\n\n\n\n");
						system("pause");
						system("cls");	
                        break;
                    case 3:     // Exibição Pós-Ordem
                        system("cls");
                        printf(BLUE"Exibição PÓS-ORDEM:\n"CRESET);
                        exibirPosOrdem(arvore);
                        printf("\n\n\n\n");
						system("pause");
						system("cls");	
                        break;
                    case 4:     // Exibição Gráfica
                        system("cls");
                        printf(BLUE"Exibição Gráfica:\n\n\n"CRESET);
                        exibirGraficamente(arvore, 60, 1, 25);
                        printf("\n\n\n\n");
						system("pause");
						system("cls");
                        break;
                    default:
                        printf(RED"Opção Inválida! Digite uma opção válida e tente novamente."CRESET);
                        break;
                }
                break;
            case 6:     // Exibição dos créditos
                system("cls");
                lerArquivo("../creditos.txt");
                system("pause");
                system("cls");
                break;
            case 0:     // Saída do programa
                esvaziar(&arvore);
                printf("Programa encerrado.");
                return 0;
            default:
                printf(RED"Opção Inválida! Digite uma opção válida e tente novamente."CRESET);
                break;
        }   
    }
}