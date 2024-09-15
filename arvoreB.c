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
    char nome[10];
    int quantiaChaves;
    int folha;
    int *chaves;
    char **filhos;
} arvoreBNo;

// arquivos serao escritos respectivamente a ordem da arvoreBNo

int random_number(){
    srand(time(NULL));
    int r = rand() % 10000000 + 10000000;
    return r;
}

void escrita(arvoreBNo* no, char nomeNo[]){

    char nomeFilhoProcessado[50];
    sprintf( nomeFilhoProcessado, "%s", nomeNo);
    FILE* arquivoNo = fopen(nomeFilhoProcessado, "wb+");

    fwrite( no->nome,  sizeof(char[10]), 1 , arquivoNo );
    printf("escrito nome: %s\n", no->nome);

    fclose(arquivoNo);
    arquivoNo = fopen(nomeFilhoProcessado, "ab+");

    fwrite( &no->quantiaChaves, sizeof(int), 1, arquivoNo);
    printf("escrito qntchv: %d\n", no->quantiaChaves);

    fwrite( &no->folha, sizeof(int), 1,  arquivoNo);
    printf("escrito folha: %d\n", no->folha);

    fwrite( &no->chaves, sizeof(int), 2*T - 1,  arquivoNo);
    for(int i = 0; i < no->quantiaChaves; i++){
    printf("escrito num%d: %d\n",i,no->chaves[i]);
    }

    fwrite( &no->filhos, sizeof(char[10]), 2*T,  arquivoNo);

}

arvoreBNo* leitura(char* nomeFilho){

    printf("nomepraler: %s\n",nomeFilho);
    char nomeFilhoProcessado[50];
    sprintf( nomeFilhoProcessado, "%s", nomeFilho);
    FILE* arquivoFilho = fopen(nomeFilhoProcessado, "rb+");
    arvoreBNo* noFilho = (arvoreBNo*) malloc(sizeof(arvoreBNo));

    char nome[50];
    fseek(arquivoFilho, 0, SEEK_SET);
    fread(&nome, sizeof(char[10]), 1, arquivoFilho);
    strcpy(noFilho->nome, nome);
    printf("lido nome: %s\n",nome);

    int qntChaves;
    fseek(arquivoFilho, sizeof(char[10]), SEEK_SET);
    fread(&qntChaves, sizeof(int),1, arquivoFilho); 
    noFilho->quantiaChaves = qntChaves;
    printf("lido qntch: %d\n",qntChaves);

    int folha;
    fseek(arquivoFilho, sizeof(char[10]) + sizeof(int), SEEK_SET);
    fread(&folha, sizeof(bool), 1,arquivoFilho);
    noFilho->folha = folha;
    printf("lido folha: %d\n",folha);

    int *chaves;
    fseek(arquivoFilho, sizeof(char[10]) + sizeof(int) + sizeof(bool), SEEK_SET);
    fread(&chaves, sizeof(int), 2*T - 1, arquivoFilho);
    noFilho->chaves = chaves;

    char **filhos;
    fseek(arquivoFilho, sizeof(char[10]) + sizeof(int) + sizeof(bool) + sizeof(int)*2*T - sizeof(int), SEEK_SET);
    fread(&filhos, sizeof(char[10]), 2*T, arquivoFilho);
    noFilho->filhos = filhos;

    return noFilho;
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

arvoreBNo* criarArvore(){
    arvoreBNo* no = (arvoreBNo*) malloc(sizeof(arvoreBNo));
    no->quantiaChaves = 0;
    no->chaves = (int*)malloc(2*T * sizeof(int) - 1);
    no->filhos = (char**) malloc((2*T) * sizeof(char*));
    no->folha = 1;
 
 // colocando o nome da raiz no descritor e no nó
    char nomeRaiz[50];
    sprintf( nomeRaiz, "%d.bin", random_number());
    strcpy(no->nome, nomeRaiz);

    int chave;
    printf("Digite um numero: ");
    scanf("%d",&chave);

  
    inserirNaoCheioArvoreB(no, chave);

    FILE* descritor = fopen(nomeDoDescritor, "wb+");
    fseek(descritor, sizeof(int), SEEK_SET);
    fwrite(nomeRaiz, sizeof(char[10]), 1, descritor);

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
        if(chave != -999){        
            inserirArvoreB(raiz,chave);
        }
    }while(chave != -999);
}

void printarArvore(){ // wip

    FILE* arquivoDescritor = fopen(nomeDoDescritor, "rb+");
    char *nomeRaiz;
    fseek(arquivoDescritor, sizeof(int), SEEK_SET);
    fread(&nomeRaiz, sizeof(char[10]), 1 , arquivoDescritor);
    arvoreBNo  = leitura( nomeRaiz );
    printf("nom%s\n", raiz->nome);
    printf("qnt%d\n", raiz->quantiaChaves);
    printf("fol%d\n",raiz->folha);
  //  for(int i = 0; i < raiz->quantiaChaves; i++){
//        printf("%d",raiz->chaves[i]);
    //}
    printf("end\n");
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
        char *nomeRaiz;
        switch(escolha){
            case 1:
                arquivoDescritor = criarDescritor();
                raiz = criarArvore();
                mainArvore(raiz);
                break;
            case 2:
                printf("Nome do arquivo: ");
                scanf("%s", nomeDoDescritor);
                arquivoDescritor = fopen(nomeDoDescritor, "wb+");
                fseek(arquivoDescritor, sizeof(int), SEEK_SET);
                fread(&nomeRaiz, sizeof(char[10]), 1 , arquivoDescritor);
                raiz = leitura(nomeRaiz);

                mainArvore(raiz);
                break;
        
            case 3:
                
                printarArvore();
                break;

            case 4:
                fseek(arquivoDescritor, sizeof(int), SEEK_SET);
                fread(&nomeRaiz, sizeof(char[10]), 1 , arquivoDescritor);
                raiz = leitura(nomeRaiz);
                
                printf("Qual numero buscas?\n");
                int chave;
                scanf("%d",&chave);
                BuscaArvoreB(raiz, chave);
                break;


        }

    }while(escolha != 0);
