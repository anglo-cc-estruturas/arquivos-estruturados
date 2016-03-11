#include <stdio.h>
#include <stdlib.h>

#define SIZE_NAME 64
#define SIZE_DOC 16

#define CLEAR "clear"
#define PAUSE "read a"

#define ARQ_PESSOAS "cad_pessoas.dat"

#define ERR_OPEN_FILE_RW 1
#define ERR_OPEN_FILE_RO 2
#define ERR_EOF 3

/* Finalizar programa incluido as seguintes opcoes
    - menu de opcoes
    - Gravar cadastro
    - Imprimir todos os cadastro
    - Imprimir um cadastro especifico informado pelo usuario
    - Pesquisar e imprimir um cadastro baseado no documento
*/


typedef struct pessoa {
    char nome[SIZE_NAME];
    int idade;
    char documento[SIZE_DOC];
    char sexo;
}tpPessoa;

void clear_buffer_stdin(){
    // Linux
    __fpurge(stdin);
    // Windows
    //fflush(stdin);
}

void ler_pessoa(tpPessoa *pessoa){
    system(CLEAR);

    clear_buffer_stdin();
    printf("Informe o nome da pessoa: ");
    scanf("%[^\n]s",pessoa->nome);

    clear_buffer_stdin();
    printf("Informe a idade da pessoa: ");
    scanf("%d",&pessoa->idade);

    clear_buffer_stdin();
    printf("Informe o documento da pessoa: ");
    scanf("%[^\n]s",pessoa->documento);

    clear_buffer_stdin();
    printf("Informe o sexo da pessoa [M/F]: ");
    scanf("%c",&pessoa->sexo);

}

void imprime_pessoa(tpPessoa pessoa){
    system(CLEAR);

    printf("\n-------------------------------------------\n");

    printf("Nome da pessoa: %s\n",pessoa.nome);

    printf("Idade da pessoa: %d\n",pessoa.idade);

    printf("Documento da pessoa: %s\n",pessoa.documento);

    printf("Sexo da pessoa: %c",pessoa.sexo);

    printf("\n-------------------------------------------\n");

}

void abrir_arq_pessoas(char arq_nome[],char modo[], FILE **arq){

    if ((*arq) = fopen(arq_nome,modo)){

    }else{
        arq = NULL;
    }

}

void imprime_erros(int cod_erro){
    switch (cod_erro){
        case ERR_OPEN_FILE_RW: {
            printf("\nNao foi possivel abrir o arquivo (ERR_OPEN_FILE_RW)\n");
            break;
        }
        case ERR_OPEN_FILE_RO: {
            printf("\nNao foi possivel abrir o arquivo (ERR_OPEN_FILE_RO)\n");
            break;
        }
        case ERR_EOF: {
            printf("\nPosicao inexistente, fim do arquivo (ERR_EOF)\n");
            break;
        }
        default: {
            printf("\nErro desconhecido!!!!\n");
        }
    }
}

void gravar_pessoa(tpPessoa pessoa){
    FILE *arq;

    abrir_arq_pessoas(ARQ_PESSOAS,"ab", &arq);

    if (arq != NULL){

        fwrite(&pessoa, sizeof(tpPessoa),1,arq);

        fclose(arq);
    }else{
        imprime_erros(ERR_OPEN_FILE_RW);
    }


}

int ler_pessoa_arquivo(tpPessoa *pessoa, int posicao){
     FILE *arq;
     int status=0;

     abrir_arq_pessoas(ARQ_PESSOAS,"rb", &arq);

     if (arq != NULL){


        fseek(arq,sizeof(tpPessoa) * posicao,SEEK_SET);

        fread(pessoa, sizeof(tpPessoa),1,arq);

        if (!feof(arq)){
            status = 1;
        }else{
            imprime_erros(ERR_EOF);
            status = 0;
        }

        fclose(arq);


    }else{
        imprime_erros(ERR_OPEN_FILE_RO);
        status = 0;
    }

    return status;
}

int main()
{
    tpPessoa pessoa;


    //printf("Tamanho de tpPessoa %li",sizeof(tpPessoa));

    //ler_pessoa(&pessoa);

    //gravar_pessoa(pessoa);

    if(ler_pessoa_arquivo(&pessoa,1))
        imprime_pessoa(pessoa);

    return 0;
}
