#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_NAME 64
#define SIZE_DOC 16

#define CLEAR "clear"
#define PAUSE "read a"

#define ARQ_PESSOAS "cad_pessoas.dat"
#define INDEX_DOC "index_doc"

#define ERR_OPEN_FILE_RW 1
#define ERR_OPEN_FILE_RO 2
#define ERR_EOF 3

/* Finalizar programa incluido as seguintes opcoes
    - menu de opcoes (deixar o programa usável)
    - Imprimir um cadastro especifico informado pelo usuario
    - Pesquisa indexada por nome
    - Excluir registros logicamente (marcar como excluido)
    - Excluir fisicamente (gerar novo arquivo com todos menos os marcados como excluidos)
        - Após excluir fisicamente, será necessario regerar indices
    - Listagem de todos os cadastros ordenados alfabeticamente pelo nome
        para isso será necessrio um arquivo de indice organizado em ordem alfabetica
*/


typedef struct pessoa {
    char nome[SIZE_NAME];
    int idade;
    char documento[SIZE_DOC];
    char sexo;
}tpPessoa;

typedef struct doc_index {
    char documento[SIZE_DOC];
    fpos_t posicao;
}tpDocIndex;

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
    //system(CLEAR);

    printf("\n-------------------------------------------\n");

    printf("Nome da pessoa: %s\n",pessoa.nome);

    printf("Idade da pessoa: %d\n",pessoa.idade);

    printf("Documento da pessoa: %s\n",pessoa.documento);

    printf("Sexo da pessoa: %c",pessoa.sexo);

    printf("\n-------------------------------------------\n");

}

void abrir_arquivo(char arq_nome[],char modo[], FILE **arq){

    if (((*arq) = fopen(arq_nome,modo))){

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
            printf("\nNao foi possivel abrir fpos_t pos; o arquivo (ERR_OPEN_FILE_RO)\n");
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
    FILE *index_doc;
    tpDocIndex doc;

    abrir_arquivo(ARQ_PESSOAS,"ab", &arq);
    abrir_arquivo(INDEX_DOC,"ab", &index_doc);


    if ((arq != NULL) && (index_doc != NULL)){
        strcpy(doc.documento,pessoa.documento);

        fgetpos(arq,&doc.posicao);

        fwrite(&pessoa, sizeof(tpPessoa),1,arq);
        fwrite(&doc, sizeof(tpDocIndex),1,index_doc);

        fclose(arq);
        fclose(index_doc);
    }else{
        imprime_erros(ERR_OPEN_FILE_RW);
    }


}

int ler_pessoa_arquivo_fseek(tpPessoa *pessoa, int posicao_fseek){
     FILE *arq;
     int status=0;

     abrir_arquivo(ARQ_PESSOAS,"rb", &arq);

     if (arq != NULL){

        fseek(arq,sizeof(tpPessoa) * posicao_fseek,SEEK_SET);

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

int ler_pessoa_arquivo_fsetpos(tpPessoa *pessoa, fpos_t posicao_fpos){
     FILE *arq;
     int status=0;

     abrir_arquivo(ARQ_PESSOAS,"rb", &arq);

     if (arq != NULL){

        fsetpos(arq,&posicao_fpos);

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

void imprime_todas_pessoas(){
    tpPessoa pessoa;
    FILE *arq;

    abrir_arquivo(ARQ_PESSOAS,"rb", &arq);

    if (arq != NULL){

        fread(&pessoa, sizeof(tpPessoa),1,arq);

        while (!feof(arq)){
            imprime_pessoa(pessoa);
            fread(&pessoa, sizeof(tpPessoa),1,arq);
        }

        fclose(arq);


    }else{
        imprime_erros(ERR_OPEN_FILE_RO);
    }

}

int localizar_pessoa_documento(char documento[SIZE_DOC],fpos_t *posicao){
    FILE *index_doc;
    tpDocIndex doc;
    int achou = 0;

    abrir_arquivo(INDEX_DOC,"rb", &index_doc);

    if (index_doc != NULL){

        fread(&doc, sizeof(tpDocIndex),1,index_doc);
        while (!feof(index_doc)){
            if (strcmp(documento,doc.documento) == 0){
                *posicao = doc.posicao;
                achou = 1;
                break;
            }else{
                fread(&doc, sizeof(tpDocIndex),1,index_doc);
            }
        }
        fclose(index_doc);
    }else{
        imprime_erros(ERR_OPEN_FILE_RO);
    }

    return achou;
}

void buscar_pessoa_documento(){
    char documento[SIZE_DOC];
    fpos_t posicao;
    tpPessoa pessoa;

    printf("Informe o documento a ser pesquisado: ");
    clear_buffer_stdin();
    scanf("%[^\n]s",documento);

    if (localizar_pessoa_documento(documento,&posicao)){
        ler_pessoa_arquivo_fsetpos(&pessoa,posicao);
        imprime_pessoa(pessoa);
    }else{
        printf("\nDocumento nao encontrado\n");
    }

}

int main(){

    tpPessoa pessoa;


    //printf("Tamanho de tpPessoa %li",sizeof(tpPessoa));

    //ler_pessoa(&pessoa);

    //gravar_pessoa(pessoa);

    //imprime_todas_pessoas();

    buscar_pessoa_documento();

    //if(ler_pessoa_arquivo_fseek(&pessoa,1))
    //    imprime_pessoa(pessoa);

    return 0;
}
