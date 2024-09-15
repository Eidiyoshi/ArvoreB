#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

char nomeDoDescritor[50]; // variaveis globais pra facilitar minha vida
int T;                    // eu troco conforme eu mudo de arquivo

// no descritor contera o T como primeiro caracter
// e logo em seguida o nome da raiz

typedef struct arvoreBNo {
    char nome[50];
    int quantiaChaves;
    int folha;
    int *chaves;
    char **filhos;
} arvoreBNo;

// arquivos serao escritos respectivamente a ordem da arvoreBNo

int random_number(){
    srand(time(NULL));
    int r = rand();
    return r;
}

void escrita(arvoreBNo* no, char nomeNo[]){
    FILE* arquivoNo = fopen(nomeNo, "wb+");

    fwrite( &no->nome, 1 , sizeof(char[50]), arquivoNo );

    fseek(arquivoNo, sizeof(char[50]), SEEK_SET);
    fwrite( &no->quantiaChaves, 1, sizeof(int), arquivoNo);
    
    fseek(arquivoNo, sizeof(char[50])+ sizeof(int), SEEK_SET);
    fwrite( &no->folha, sizeof(int), 1,  arquivoNo);
    
    fseek(arquivoNo, sizeof(char[50]) + sizeof(int) + sizeof(bool), SEEK_SET);
    fwrite( &no->chaves, sizeof(int), 2*T - 1,  arquivoNo);
    
    fseek(arquivoNo, sizeof(char[50]) + sizeof(int) + sizeof(bool) + sizeof(int)*2*T, SEEK_SET);
    fwrite( &no->filhos, sizeof(char[50]), 2*T,  arquivoNo);

}

arvoreBNo* leitura(char* nomeFilho){
    
    char nomeFilhoProcessado[50];
    sprintf( nomeFilhoProcessado, "%s", nomeFilho);
    
    FILE* arquivoFilho = fopen(nomeFilhoProcessado, "wb+");
    arvoreBNo* noFilho = (arvoreBNo*) malloc(sizeof(arvoreBNo));

    char nome[50];
    fread(&nome, sizeof(char[50]), 1, arquivoFilho);
    strcpy(noFilho->nome, nome);

    int qntChaves;
    fseek(arquivoFilho, sizeof(char[50]), SEEK_SET);
    fread(&qntChaves, sizeof(int),1, arquivoFilho); 
    noFilho->quantiaChaves = qntChaves;

    int folha;
    fseek(arquivoFilho, sizeof(char[50]) + sizeof(int), SEEK_SET);
    fread(&folha, sizeof(bool), 1,arquivoFilho);
    noFilho->folha = folha;

    int *chaves;
    fseek(arquivoFilho, sizeof(char[50]) + sizeof(int) + sizeof(bool), SEEK_SET);
    fread(&chaves, sizeof(int), 2*T - 1, arquivoFilho);
    noFilho->chaves = chaves;

    char **filhos;
    fseek(arquivoFilho, sizeof(char[50]) + sizeof(int) + sizeof(bool) + sizeof(int)*2*T - sizeof(int), SEEK_SET);
    fread(&filhos, sizeof(char[50]), 2*T, arquivoFilho);
    noFilho->filhos = filhos;

    return noFilho;
}

arvoreBNo* criarArvore(){
    arvoreBNo* no = (arvoreBNo*) malloc(sizeof(arvoreBNo));
    no->quantiaChaves = 0;
    no->chaves = (int*)malloc(2*T * sizeof(int) - 1);
    no->filhos = (char**) malloc((2*T) * sizeof(char*));
    no->folha = 1;

    // colocando o nome da raiz no descritor
    char nomeRaiz[50];
    sprintf( nomeRaiz, "%d.bin", random_number());
    strcpy(no->nome, nomeRaiz);

    FILE* descritor = fopen(nomeDoDescritor, "wb+");
    fseek(descritor, sizeof(int), SEEK_SET);
    fwrite(nomeRaiz, sizeof(char[50]), 1, descritor);

    // colocando o nó no arquivo da raiz
    FILE* arquivoRaiz = fopen(nomeRaiz, "wb+");
    escrita(no, nomeRaiz);

    return no;
}

arvoreBNo* criarNo(int folha) {
    arvoreBNo* no = (arvoreBNo*) malloc(sizeof(arvoreBNo));
    no->quantiaChaves = 0;
    no->chaves = (int*)malloc( (2*T-1) * sizeof(int) );
    no->filhos = (char**) malloc((2*T) * sizeof(char*));
    no->folha = folha;
    return no;
}


FILE* criarDescritor(){
    printf("Nome do Arquivo: ");
    scanf("%s",nomeDoDescritor);
    FILE* arquivo = fopen(nomeDoDescritor, "wb+");
    
    do{
    printf("Grau t da Raiz: ");
    fflush(stdin);
    scanf("%d",&T);
    }while( T < 1 );

    fputc(T, arquivo);
    fclose(arquivo);
    return arquivo;
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
        escrita(raiz, raiz->nome);
    }else{
        while( contadorReverso >= 1 && chave < raiz->chaves[contadorReverso] ){
            contadorReverso--;
        }
        contadorReverso++;
        arvoreBNo* filhoI = leitura(raiz->filhos[contadorReverso]);
    }
}

void inserirArvoreB( arvoreBNo *raiz, int chave){
    if( raiz->quantiaChaves == 2*T - 1 ){
        printf("oi");
        return;
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

void printarArvore(arvoreBNo *raiz){ //wip
    FILE* arquivo = fopen(raiz->nome, "wb+");

    fread(&raiz, sizeof(arvoreBNo), 1, arquivo);

    for(int i = 0; i < raiz->quantiaChaves; i++){
        printf("%d",raiz->chaves[i]);
    }
    printf("end");
}


void BuscaArvoreB(arvoreBNo *no, int chave){
    int i = 1;
    printf("a");
    
    while ( i <= no->quantiaChaves &chave > no->chaves[i] ){
        i++;
    }
    printf("b");
    
    if( (i <= no->quantiaChaves) && chave == no->chaves[i] ){
        printf("Esta na arvore\n");
        return;
    }
    
    if(no->folha){
        printf("Nao esta na arvore\n");
        return;
    }
    
    //arvoreBNo *filho = leitura(no->filhos[i]);
   // BuscaArvoreB(filho, chave);

}

void main(){
    int escolha;
    FILE* arquivoDescritor = NULL;

    printf("Bem vindo ao trabalho de Estrutura de Dados 2\n");


    do{
        printf("1- Criar nova arvore\n");
        printf("2- Abrir arvore\n");
        printf("3- Printar arvore atual\n");
        printf("4- Verificar elemento\n");
        printf("0- Fechar programa\n");

        printf("Escolha: ");
        scanf("%d",&escolha);
        arvoreBNo *raiz = NULL;
        switch(escolha){
            case 1:
                arquivoDescritor = criarDescritor();
                raiz = criarArvore();
                mainArvore(raiz);
                break;
            case 2:
                break;
        
            case 3:
                printarArvore(raiz);
                break;

            case 4:
                char nomeRaiz[50];
                fseek(arquivoDescritor, sizeof(int), SEEK_SET);
                fread(&nomeRaiz, sizeof(char[50]), 1 , arquivoDescritor);
                raiz = leitura(nomeRaiz);
                printf("Qual numero buscas?\n");
                int chave;
                scanf("%d",&chave);
                BuscaArvoreB(raiz, chave);
                break;


        }

    }while(escolha != 0);

}
