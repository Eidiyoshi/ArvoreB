#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// vou botar o T no primeiro caracter da raiz

typedef struct arvoreBNo {
    int quantiaChaves;
    int *chaves;
    char **filhos;
    bool folha;
} arvoreBNo;


arvoreBNo* criarNo(int T, bool folha) {
    arvoreBNo* no = (arvoreBNo*) malloc(sizeof(arvoreBNo));
    no->quantiaChaves = 0;
    no->chaves = (int*) malloc((2*T - 1) * sizeof(int));
    no->filhos = (char**) malloc((2*T) * sizeof(char*));
    no->folha = folha;
    return no;
}


FILE* criarArquivo(){
    char nomeDoArquivo[50];
    printf("Nome do Arquivo: ");
    scanf("%s",nomeDoArquivo);
    FILE* arquivo = fopen(nomeDoArquivo, "wb+");
    printf("Grau t da Raiz: ");
    int t;
    fflush(stdin);
    scanf("%d",&t);
    fputc(t, arquivo);
    fclose(arquivo);
    return arquivo;
}

void mainArvore(int T){
    int input;
    do{
        printf(" -999 para sair\n");
        printf("Insira um numero: ");
        scanf("%d",&input);

    }while(input != -999);
}

void main(){

    printf("Bem vindo ao trabalho de Estrutura de Dados 2\n");
    printf("1- Criar nova arvore\n");
    printf("2- Abrir arvore\n");
    printf("0- Fechar programa\n");

    int escolha;
    int T;
    FILE* arquivoDescritor = NULL;


    do{
        printf("Escolha: ");
        scanf("%d",&escolha);
        switch(escolha){
            case 1:
                arquivoDescritor = criarArquivo(T);
                mainArvore(T);
                break;
            case 2:
                break;

        }

    }while(escolha != 0);

}
