#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// vou botar o T no primeiro caracter da raiz
char nomeDoDescritor[50]; // variaveis globais pra facilitar minha vida
int T;                    // eu troco conforme eu mudo de arquivo


typedef struct arvoreBNo {
    int nome;
    int quantiaChaves;
    int *chaves;
    char **filhos;
    bool folha;
} arvoreBNo;

int random_number(){
    srand(time(NULL));
    int r = rand();
    return r;
}

arvoreBNo* criarArvore(){
    arvoreBNo* no = (arvoreBNo*) malloc(sizeof(arvoreBNo));
    no->quantiaChaves = 0;
    int *chaves[2*T-1];
    memset(chaves, 0, sizeof(int)*(2*T-1));
    no->chaves = chaves;
    no->filhos = (char**) malloc((2*T) * sizeof(char*));
    no->folha = true;
    no->nome = random_number();

    return no;
}

arvoreBNo* criarNo(bool folha) {
    arvoreBNo* no = (arvoreBNo*) malloc(sizeof(arvoreBNo));
    no->quantiaChaves = 0;
    int *chaves[2*T-1];
    memset(chaves, 0, sizeof(int)*(2*T-1));
    no->chaves = chaves;
    no->filhos = (char**) malloc((2*T) * sizeof(char*));
    no->folha = folha;
    return no;
}


FILE* criarArquivo(){
    printf("Nome do Arquivo: ");
    scanf("%s",nomeDoDescritor);
    FILE* arquivo = fopen(nomeDoDescritor, "wb+");
    printf("Grau t da Raiz: ");
    fflush(stdin);
    scanf("%d",&T);
    fputc(T, arquivo);
    fclose(arquivo);
    return arquivo;
}

void escreverNumArquivo(arvoreBNo* raiz){
    FILE* arquivoNo = fopen(raiz->nome, "wb+");
    fwrite(raiz, sizeof(arvoreBNo)*2*T-1, 1 , arquivoNo);
}

void inserirNaoCheioArvoreB( arvoreBNo *raiz, int chave){
    int contadorReverso = raiz->quantiaChaves;
    if( raiz->folha ){
        while( ( contadorReverso >= 1 ) && ( chave < (raiz->chaves[contadorReverso]) ) ){
            raiz->chaves[contadorReverso+1] = raiz->chaves[contadorReverso];
            contadorReverso--;
        }
        raiz->chaves[contadorReverso+1] = chave;
        raiz->quantiaChaves = raiz->quantiaChaves+1;
        escreverNumArquivo(raiz);
    }
}

void inserirArvoreB( arvoreBNo *raiz, int chave){
    if( raiz->quantiaChaves == 2*T - 1 ){

    }
    else{
        inserirNaoCheioArvoreB(raiz, chave);
    }
}

void mainArvore(arvoreBNo *raiz){
    int chave;
    do{
        printf(" -999 para sair\n");
        printf("Insira um numero: ");
        scanf("%d",&chave);
        inserirArvoreB(raiz,chave);

    }while(chave != -999);
}



void main(){

    printf("Bem vindo ao trabalho de Estrutura de Dados 2\n");
    printf("1- Criar nova arvore\n");
    printf("2- Abrir arvore\n");
    printf("0- Fechar programa\n");

    int escolha;
    FILE* arquivoDescritor = NULL;


    do{
        printf("Escolha: ");
        scanf("%d",&escolha);
        switch(escolha){
            case 1:
                arquivoDescritor = criarArquivo();
                arvoreBNo *raiz = criarArvore();
                mainArvore(raiz);
                break;
            case 2:
                break;

        }

    }while(escolha != 0);

}
