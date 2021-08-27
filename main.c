#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char currState[11];
    char currSymbol[4];
    char newSymbol[4];
    char direction[3];
    char newState[11];
    int movidos;
} Mt;

typedef struct {
    char currState[11];
    char currSymbol[4];
    char newSymbol[4];
    char direction[3];
    char newState[11];
    int ativo;
} MtPendente;


Mt **criarEstado(char *palavraAtual) { //aqui eu crio os estados da maquina a partir das linhas do arquivo de texto
    Mt *mt = malloc(sizeof(Mt));

    int i=0;

    while(palavraAtual[i]!=' ') {
        mt->currState[i] = palavraAtual[i];
        i++;
    }
    mt->currState[i] ='\0';
    i++;

    int j=0;

    while(palavraAtual[i] != ' ') {
        mt->currSymbol[j] = palavraAtual[i];
        i++;
        j++;
    }
    mt->currSymbol[j] ='\0';

    i++;
    j=0;

    while(palavraAtual[i] != ' ') {
        mt->newSymbol[j] = palavraAtual[i];
        i++;
        j++;
    }
    mt->newSymbol[j] ='\0';
    i++;
    j=0;

    while(palavraAtual[i] != ' ') {
        mt->direction[j] = palavraAtual[i];
        j++;
        i++;
    }
    mt->direction[j] ='\0';

    i++;
    j=0;
    while(palavraAtual[i] !='\0' && palavraAtual[i] != '\n') {
        mt->newState[j] = palavraAtual[i];
        j++;
        i++;
    }
    mt->newState[j] ='\0';

    i=0;

    while(palavraAtual[i] >=45) {
        palavraAtual[i] = '\0';
        i++;
    }

    return mt;
}



Mt *lerMaquina(int *tam) { //aqui eu leio o arquivo de texto
    Mt **mt = malloc(sizeof(Mt));
    int i=0, j=0;

    FILE *fp = fopen("maquinaEntrada.in", "r");

    if(fp == NULL) {
        printf("Arquivo 'maquinaEntrada.in' nao encontrado\n");
        return NULL;
    }

    char c;
    char *palavraAtual = malloc(sizeof(char)*200);

    while((fgets(palavraAtual, 200, fp)!=NULL)) {
        if(palavraAtual[0] > 10){
            mt[j] = criarEstado(palavraAtual);
            j++;
            mt = realloc(mt, sizeof(Mt)*j+1);
        }
    }

    fclose(fp);
    tam[0] = j;
    return mt;
}

void retornarNovoEstado(char *novoEstado, Mt **mt, int *tam, Mt **mt2) { //essa função retorna estados auxiliares, para simular movimentos estacionarios ou algo do tipo
    int i=0, random1 =0, random2 =0, j=0;

    while(novoEstado[i] !='\0') {
        novoEstado[i] = '\0';
        i++;
    }

    while(i<tam[0]) {
        if(i == 0) {
            srand((unsigned) time(NULL));
            random1 = 49 + rand() % 9  ;
            srand((unsigned) time(NULL));
            random2 = 97 + rand() % 25;
            j=0;

            while(j<tam[1]-1) {
                if(random1 == mt2[j]->currState[0] && random2 == mt2[j]->currState[1]) {
                    i=-1;
                    j= tam[1];
                    if(random1 < 58) {
                        random1++;
                    } else {
                        random1--;
                    }
                }
                j++;
            }
        }
        if(i!= -1){
            if(random1 == mt[i]->currState[0] && random2 == mt[i]->currState[1]) {
                i=-1;
            }
        }

        if(random1 == 0  || random2 ==0 && i == tam[0]-1) {
            i = -1;
        }
        i++;
    }
    novoEstado[0] = random1;
    novoEstado[1] = random2;
    novoEstado[2] =  '\0';
}

void limparString(char *string) { //limpar as strings auxiliares, para n ficar indo lixo
    int i=0;

    while(string[i]!='\0') {
        string[i] = '\0';
        i++;
    }
}

Mt **traduzirMt(Mt **maquinaRecebida, int *tam, Mt **maquinaInicial, char **estados, MtPendente ** mtPendente) { //sei que essa função ta muito feia, mas é aqui que a maquina é traduzida
    int i=0, j=0, k=0, p=0, z=0, u=0;
    char *novoEstado = malloc(sizeof(char)*3);
    char *palavra = malloc(sizeof(char)*100);
    Mt **maquinaSaida = malloc(sizeof(Mt*)*4);
    tam[1] = 5;
    for(i=0;i<4;i++) {
        maquinaSaida[i] = malloc(sizeof(Mt));

        strcpy(maquinaSaida[i]->currState, maquinaInicial[i]->currState);
        strcpy(maquinaSaida[i]->currSymbol, maquinaInicial[i]->currSymbol);
        strcpy(maquinaSaida[i]->direction, maquinaInicial[i]->direction);
        strcpy(maquinaSaida[i]->newState, maquinaInicial[i]->newState);
        strcpy(maquinaSaida[i]->newSymbol, maquinaInicial[i]->newSymbol);
    }

    i=0;
    while(i < tam[0]) {

        if(i == tam[0]/2) {
            printf("calma, eu nao entrei em loop\n");
        }

        if(p < tam[2]-1){
            if(strcmp(maquinaRecebida[i]->currState, mtPendente[p]->currState) == 0 && mtPendente[p]->ativo == 1) {
                j = tam[1]-1;
                maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                maquinaSaida[j] = malloc(sizeof(Mt));
                strcpy(maquinaSaida[j]->currState, mtPendente[p]->currState);
                strcpy(maquinaSaida[j]->currSymbol, mtPendente[p]->currSymbol);
                strcpy(maquinaSaida[j]->direction, mtPendente[p]->direction);
                strcpy(maquinaSaida[j]->newState, mtPendente[p]->newState);
                strcpy(maquinaSaida[j]->newSymbol, mtPendente[p]->newSymbol);
                tam[1]++;
                j++;
                p++;
            }
        }
            j = tam[1] -1;
            if(maquinaRecebida[i]->currSymbol[0] == '_') { //verificar se to lendo o simbolo de branco
                if(maquinaRecebida[i]->newSymbol[0] == '_') { //verificar se era pra escrever o simbolo de branco
                    if(maquinaRecebida[i]->direction[0] == '*') { //verificar se é movimento estacionario
                        j = tam[1]-1;
                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState,maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "_\0");
                        strcpy(maquinaSaida[j]->direction, "r\0");
                        retornarNovoEstado(novoEstado, maquinaRecebida, tam, maquinaSaida);
                        strcpy(maquinaSaida[j]->newState, novoEstado);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState,maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "r\0");
                        strcpy(maquinaSaida[j]->newState, novoEstado);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState, novoEstado);
                        strcpy(maquinaSaida[j]->currSymbol, "_\0");
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState, novoEstado);
                        strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        tam[1]++;
                        j++;

                        for(k=0; k<tam[3]-1; k++) {
                            maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                            maquinaSaida[j] = malloc(sizeof(Mt));
                            strcpy(maquinaSaida[j]->currState, novoEstado);
                            strcpy(maquinaSaida[j]->currSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->newSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->direction, "l\0");
                            strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                            tam[1]++;
                            j++;
                        }
                        limparString(novoEstado);

                    } else if (maquinaRecebida[i]->direction[0] == 'l') { //aqui ainda é lendo e escrevendo branco, mas movimento pra esquerda
                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        j=tam[1]-1;
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "_\0");
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        tam[1]++;
                        j++;

                    }  else { //aqui lendo e escrevendo branco, mas com movimento pra direito, poderia juntar os dois, mas n quero mais mexer no codigo
                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        j=tam[1]-1;
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "_\0");
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "r\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        j=tam[1]-1;
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "r\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        tam[1]++;
                        j++;
                    }
                } else { //aqui é lendo branco e escrevendo qualquer outra coisa q n seja branco
                    if(maquinaRecebida[i]->direction == '*') { //lendo branco, escrevendo qualquer coisa com simbolo estacionario
                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        j=tam[1]-1;
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState,maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "_\0");
                        strcpy(maquinaSaida[j]->direction, "r\0");
                        retornarNovoEstado(novoEstado, maquinaRecebida, tam, maquinaSaida);
                        strcpy(maquinaSaida[j]->newState, novoEstado);
                        strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "r\0");
                        strcpy(maquinaSaida[j]->newState, novoEstado);
                        strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                        tam[1]++;
                        j++;

                        for(k=0; k<tam[3]-1; k++) {
                            maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                            maquinaSaida[j] = malloc(sizeof(Mt));
                            strcpy(maquinaSaida[j]->currState, novoEstado);
                            strcpy(maquinaSaida[j]->currSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->newSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->direction, "l\0");
                            strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                            tam[1]++;
                            j++;
                        }

                        limparString(novoEstado);
                    } else if(maquinaRecebida[i]->direction[0] == 'l') {//mesmo esquema, lendo branco, escrevendo qualquer coisa, e indo pra esquerda
                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        j=tam[1]-1;
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState,maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "_\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                        tam[1]++;
                        j++;
                    }else { //aqui mesma coisa  de cima, mas indo pra direita, poderia juntar os dois, mas n quero mexer no codigo
                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        j=tam[1]-1;
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState,maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "_\0");
                        strcpy(maquinaSaida[j]->direction, "r\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "r\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                        tam[1]++;
                        j++;
                    }
                }
            } else if (maquinaRecebida[i]->currSymbol[0] != '*' && maquinaRecebida[i]->currSymbol[0] != '_') { //aqui é lendo algo que nao seja _ nem  *
                if(maquinaRecebida[i]->newSymbol[0] == '_') { // se for escrever branco
                    if(maquinaRecebida[i]->direction[0] == 'l' || maquinaRecebida[i]->direction[0] == 'r') { //se nao for movimento estacionario e escrever simbolo de branco
                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        j = tam[1]-1;
                        maquinaSaida[j] = malloc(sizeof(Mt));

                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, maquinaRecebida[i]->currSymbol);
                        strcpy(maquinaSaida[j]->direction, maquinaRecebida[i]->direction);
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;
                    }else { // se for movimento estacionario e escrever simbolo de branco
                        retornarNovoEstado(novoEstado, maquinaRecebida, tam, maquinaSaida);

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        j = tam[1]-1;
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, maquinaRecebida[i]->currSymbol);
                        strcpy(maquinaSaida[j]->direction, "r\0");
                        strcpy(maquinaSaida[j]->newState, novoEstado);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, novoEstado);
                        strcpy(maquinaSaida[j]->currSymbol, "_\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, novoEstado);
                        strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;

                        for(k=0; k<tam[3]-1; k++) {
                            maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                            maquinaSaida[j] = malloc(sizeof(Mt));
                            strcpy(maquinaSaida[j]->currState, novoEstado);
                            strcpy(maquinaSaida[j]->currSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->newSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->direction, "l\0");
                            strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                            tam[1]++;
                            j++;
                        }

                        limparString(novoEstado);
                    }
                } else { //aqui é lendo algo q nao seja * e _ e escrevendo algo q n seja branco
                    if(maquinaRecebida[i]->direction[0] == '*'){ //verificando se é movimento estacionario
                        retornarNovoEstado(novoEstado, maquinaRecebida, tam, maquinaSaida);

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        j = tam[1]-1;
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, maquinaRecebida[i]->currSymbol);
                        strcpy(maquinaSaida[j]->direction, "r\0");
                        strcpy(maquinaSaida[j]->newState, novoEstado);
                        strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        j = tam[1]-1;
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, novoEstado);
                        strcpy(maquinaSaida[j]->currSymbol, "_\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        j = tam[1]-1;
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, novoEstado);
                        strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;

                        for(k=0; k<tam[3]-1; k++) {
                            maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                            maquinaSaida[j] = malloc(sizeof(Mt));
                            strcpy(maquinaSaida[j]->currState, novoEstado);
                            strcpy(maquinaSaida[j]->currSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->newSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->direction, "l\0");
                            strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                            tam[1]++;
                            j++;
                        }
                        limparString(novoEstado);
                    } else { //lendo algo q nao seja * nem branco e movimento nao estacionario
                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        j = tam[1]-1;
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, maquinaRecebida[i]->currSymbol);
                        strcpy(maquinaSaida[j]->direction, maquinaRecebida[i]->direction);
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                        tam[1]++;
                        j++;
                    }
                }
            } else if(maquinaRecebida[i]->currSymbol[0] == '*' && maquinaRecebida[i]->newSymbol[0] == '*') { //aqui é se ler e escrever qualquer coisa
                if(maquinaRecebida[i]->direction[0]!= '*') { //ler e escrever qualquer coisa com simbolo estacionario
                    j = tam[1] -1;

                    for(k=0; k<tam[3]-1; k++) {
                        z=0;
                        for(u=0;u<tam[0];u++) {
                            if(strcmp(maquinaRecebida[i]->currState, maquinaRecebida[u]->currState) == 0 && strcmp(maquinaRecebida[u]->currSymbol, estados[k]) == 0) {
                                z = 1;
                                u = tam[0];
                            }
                        }
                        if(z == 0){
                            maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                            maquinaSaida[j] = malloc(sizeof(Mt));
                            strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                            strcpy(maquinaSaida[j]->currSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->newSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->direction, maquinaRecebida[i]->direction);
                            strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                            tam[1]++;
                            j++;
                        }
                    }
                    z=0;
                    for(u=0;u<tam[0];u++) {
                        if(strcmp(maquinaRecebida[i]->currState, maquinaRecebida[u]->currState) == 0 && maquinaRecebida[u]->currSymbol[0] ==  '_') {
                            z = 1;
                            u = tam[0];
                        }
                    }

                    if(z == 0) {
                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "_\0");
                        strcpy(maquinaSaida[j]->direction, maquinaRecebida[i]->direction);
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, maquinaRecebida[i]->direction);
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;
                    }

                } else { //ler e escrever qualquer coisa com simbolo n estacionario

                    j = tam[1] -1;
                    retornarNovoEstado(novoEstado, maquinaRecebida, tam, maquinaSaida);
                    for(k=0; k<tam[3]-1; k++) {
                        z=0;
                        for(u=0;u<tam[0];u++) {
                            if(strcmp(maquinaRecebida[i]->currState, maquinaRecebida[u]->currState) == 0 && strcmp(maquinaRecebida[u]->currSymbol, estados[k]) == 0) {
                                z = 1;
                                u = tam[0];
                            }
                        }
                        if(z == 0) {
                            maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                            maquinaSaida[j] = malloc(sizeof(Mt));
                            strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                            strcpy(maquinaSaida[j]->currSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->newSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->direction, "r\0");
                            strcpy(maquinaSaida[j]->newState, novoEstado);
                            tam[1]++;
                            j++;
                        }
                    }
                    z=0;
                    for(u=0;u<tam[0];u++) {
                        if(strcmp(maquinaRecebida[i]->currState, maquinaRecebida[u]->currState) == 0 && maquinaRecebida[u]->currSymbol[0] ==  '_') {
                            z = 1;
                            u = tam[0];
                        }
                    }

                    if(z == 0) {
                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "_\0");
                        strcpy(maquinaSaida[j]->direction, "r\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                        strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "r\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;
                    }
                    for(k=0; k<tam[3]-1; k++) {
                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, novoEstado);
                        strcpy(maquinaSaida[j]->currSymbol, estados[k]);
                        strcpy(maquinaSaida[j]->newSymbol, estados[k]);
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        tam[1]++;
                        j++;
                    }
                    maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                    maquinaSaida[j] = malloc(sizeof(Mt));
                    strcpy(maquinaSaida[j]->currState, novoEstado);
                    strcpy(maquinaSaida[j]->currSymbol, "_\0");
                    strcpy(maquinaSaida[j]->direction, "l\0");
                    strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                    strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                    tam[1]++;
                    j++;

                    maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                    maquinaSaida[j] = malloc(sizeof(Mt));
                    strcpy(maquinaSaida[j]->currState, novoEstado);
                    strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                    strcpy(maquinaSaida[j]->direction, "l\0");
                    strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                    strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                    tam[1]++;
                    j++;
                    limparString(novoEstado);
                }
            } else if (maquinaRecebida[i]->currSymbol[0] == '*' && maquinaRecebida[i]->newSymbol[0] != '*') { //ler qualquer coisa e escrever um simbolo especifico
                if(maquinaRecebida[i]->newSymbol[0] == '_'){ //verificar se o simbolo especifico é branco
                    if(maquinaRecebida[i]->direction[0] != '*') { //ler qualquer coisa, escrever branco e com movimento estacionario
                        for(k=0; k<tam[3]-1; k++) {
                            z=0;
                            for(u=0;u<tam[0];u++) {
                                if(strcmp(maquinaRecebida[i]->currState, maquinaRecebida[u]->currState) == 0 && strcmp(maquinaRecebida[u]->currSymbol, estados[k]) == 0) {
                                    z = 1;
                                    u = tam[0];
                                }
                            }
                            if(z == 0){
                                maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                                maquinaSaida[j] = malloc(sizeof(Mt));
                                strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                                strcpy(maquinaSaida[j]->currSymbol, estados[k]);
                                strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                                strcpy(maquinaSaida[j]->direction, maquinaRecebida[i]->direction);
                                strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                                tam[1]++;
                                j++;
                            }
                        }
                    } else { //ler qualquer coisa, escrever branco com movimento qualquer nao estacionario
                        j = tam[1] -1;
                        retornarNovoEstado(novoEstado, maquinaRecebida, tam, maquinaSaida);
                        for(k=0; k<tam[3]-1; k++) {
                            z=0;
                            for(u=0;u<tam[0];u++) {
                                if(strcmp(maquinaRecebida[i]->currState, maquinaRecebida[u]->currState) == 0 && strcmp(maquinaRecebida[u]->currSymbol, estados[k]) == 0) {
                                    z = 1;
                                    u = tam[0];
                                }
                            }
                            if(z == 0) {
                                maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                                maquinaSaida[j] = malloc(sizeof(Mt));
                                strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                                strcpy(maquinaSaida[j]->currSymbol, estados[k]);
                                strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                                strcpy(maquinaSaida[j]->direction, "r\0");
                                strcpy(maquinaSaida[j]->newState, novoEstado);
                                tam[1]++;
                                j++;
                            }
                        }

                        for(k=0; k<tam[3]-1; k++) {
                            maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                            maquinaSaida[j] = malloc(sizeof(Mt));
                            strcpy(maquinaSaida[j]->currState, novoEstado);
                            strcpy(maquinaSaida[j]->currSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->newSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->direction, "l\0");
                            strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                            tam[1]++;
                            j++;
                        }
                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, novoEstado);
                        strcpy(maquinaSaida[j]->currSymbol, "_\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, novoEstado);
                        strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, "¢\0");
                        tam[1]++;
                        j++;
                        limparString(novoEstado);
                    }

                } else { //ler qualquer coisa, e escrever algo diferente de branco que nao seja *
                    if(maquinaRecebida[i]->direction[0] != '*'){ //ler qualquer coisa, escrever algo diferente de branco com simbolo estacionario
                        j = tam[1] -1;
                        for(k=0; k<tam[3]-1; k++) {
                            z=0;
                            for(u=0;u<tam[0];u++) {
                                if(strcmp(maquinaRecebida[i]->currState, maquinaRecebida[u]->currState) == 0 && strcmp(maquinaRecebida[u]->currSymbol, estados[k]) == 0) {
                                    z = 1;
                                    u = tam[0];
                                }
                            }
                            if(z == 0){
                                maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                                maquinaSaida[j] = malloc(sizeof(Mt));
                                strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                                strcpy(maquinaSaida[j]->currSymbol, estados[k]);
                                strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                                strcpy(maquinaSaida[j]->direction, maquinaRecebida[i]->direction);
                                strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                                tam[1]++;
                                j++;
                            }
                        }
                        z=0;
                        for(u=0;u<tam[0];u++) {
                            if(strcmp(maquinaRecebida[i]->currState, maquinaRecebida[u]->currState) == 0 && maquinaRecebida[u]->currSymbol[0] ==  '_'){
                                z = 1;
                                u = tam[0];
                            }
                        }

                        if(z == 0) {
                            maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                            maquinaSaida[j] = malloc(sizeof(Mt));
                            strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                            strcpy(maquinaSaida[j]->currSymbol, "_\0");
                            strcpy(maquinaSaida[j]->direction, maquinaRecebida[i]->direction);
                            strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                            strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                            tam[1]++;
                            j++;

                            maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                            maquinaSaida[j] = malloc(sizeof(Mt));
                            strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                            strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                            strcpy(maquinaSaida[j]->direction, maquinaRecebida[i]->direction);
                            strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                            strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                            tam[1]++;
                            j++;
                        }
                    } else {//ler qualquer coisa, escrever qualquer coisa menos branco, com simbolo diferente de estacionario
                        j = tam[1] -1;
                        retornarNovoEstado(novoEstado, maquinaRecebida, tam, maquinaSaida);
                        for(k=0; k<tam[3]-1; k++) {
                            z=0;
                            for(u=0;u<tam[0];u++) {
                                if(strcmp(maquinaRecebida[i]->currState, maquinaRecebida[u]->currState) == 0 && strcmp(maquinaRecebida[u]->currSymbol, estados[k]) == 0) {
                                    z = 1;
                                    u = tam[0];
                                }
                            }
                            if(z == 0){
                                maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                                maquinaSaida[j] = malloc(sizeof(Mt));
                                strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                                strcpy(maquinaSaida[j]->currSymbol, estados[k]);
                                strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                                strcpy(maquinaSaida[j]->direction, "r\0");
                                strcpy(maquinaSaida[j]->newState, novoEstado);
                                tam[1]++;
                                j++;
                            }
                        }


                        z=0;
                        for(u=0;u<tam[0];u++) {
                            if(strcmp(maquinaRecebida[i]->currState, maquinaRecebida[u]->currState) == 0 && maquinaRecebida[u]->currSymbol[0] ==  '_') {
                                z = 1;
                                u = tam[0];
                            }
                        }

                        if(z == 0) { //ESSES IFS COM A VARIAVEL Z, A VARIAVEL Z É UM AUXILIAR PARA VERIFICAR SE EXISTE ALGUMA DUPLICIDADE NESSE ESTADO, POIS A GNT LE QUALQUER COISA, E SE JA ESTIVER LENDO O BRANCO NESSE MESMO ESTADO, ISSO VAI FAZER O ESTADO NAO LER O BRANCO DE NOVO
                            maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                            maquinaSaida[j] = malloc(sizeof(Mt));
                            strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                            strcpy(maquinaSaida[j]->currSymbol, "_\0");
                            strcpy(maquinaSaida[j]->direction, "r\0");
                            strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                            strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                            tam[1]++;
                            j++;

                            maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                            maquinaSaida[j] = malloc(sizeof(Mt));
                            strcpy(maquinaSaida[j]->currState, maquinaRecebida[i]->currState);
                            strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                            strcpy(maquinaSaida[j]->direction, "r\0");
                            strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                            strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                            tam[1]++;
                            j++;
                        }

                        for(k=0; k<tam[3]-1; k++) {
                            maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                            maquinaSaida[j] = malloc(sizeof(Mt));
                            strcpy(maquinaSaida[j]->currState, novoEstado);
                            strcpy(maquinaSaida[j]->currSymbol, estados[k]);
                            strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                            strcpy(maquinaSaida[j]->direction, "l\0");
                            strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                            tam[1]++;
                            j++;
                        }
                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, novoEstado);
                        strcpy(maquinaSaida[j]->currSymbol, "_\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol,maquinaRecebida[i]->newSymbol);
                        tam[1]++;
                        j++;

                        maquinaSaida = realloc(maquinaSaida, sizeof(Mt)*tam[1]);
                        maquinaSaida[j] = malloc(sizeof(Mt));
                        strcpy(maquinaSaida[j]->currState, novoEstado);
                        strcpy(maquinaSaida[j]->currSymbol, "¢\0");
                        strcpy(maquinaSaida[j]->direction, "l\0");
                        strcpy(maquinaSaida[j]->newState, maquinaRecebida[i]->newState);
                        strcpy(maquinaSaida[j]->newSymbol, maquinaRecebida[i]->newSymbol);
                        tam[1]++;
                        j++;


                        limparString(novoEstado);
                    }
                }
            }

        i++;
    }

    return maquinaSaida;
}

char *acharNovoEstado(int *tam, Mt **mtRecebida) { //essa função retorna o estado que vai ser o novo estado do 0
    int i=0, random1 =0, random2=0;
    char *novoEstado = malloc(sizeof(char)*3);


    while(i<tam[0]) {
        if(i == 0) {
            srand((unsigned) time(NULL));
            random1 = 49 + rand() % 9  ;
            srand((unsigned) time(NULL));
            random2 = 97 + rand() % 25;
        }
        if(random1 == mtRecebida[i]->currState[0] && random2 == mtRecebida[i]->currState[1]) {
            i=-1;
        }
        i++;
    }
    i=0;

    while(i<tam[0]) {
        if(mtRecebida[i]->currState[0] == '0' && (mtRecebida[i]->currState[1] == '\0' || mtRecebida[i]->currState[1] == ' ')) {
            mtRecebida[i]->currState[0] = random1;
            mtRecebida[i]->currState[1] = random2;
            mtRecebida[i]->currState[2] = '\0';
        }

        if(mtRecebida[i]->newState[0] == '0' && (mtRecebida[i]->newState[1] == '\0' || mtRecebida[i]->newState[1] == ' ')) {
            mtRecebida[i] ->newState[0] = random1;
            mtRecebida[i]->newState[1] = random2;
            mtRecebida[i]->newState[2] = '\0';
        }
        i++;
    }
    i=0;

    while(i<tam[0]) {
        if(i == 0) {
            srand((unsigned) time(NULL));
            random1 = 49 + rand() % 9  ;
            srand((unsigned) time(NULL));
            random2 = 97 + rand() % 25;
        }

        if(random1 == mtRecebida[i]->currState[0] && random2 == mtRecebida[i]->currState[1]) {
            i=-1;
        }
        i++;
    }

    novoEstado[0] = random1;
    novoEstado[1] = random2;
    novoEstado[2] = '\0';

    return novoEstado;
}


Mt **iniciaMaquinaInicial(int *tam, char *novoEstado, Mt **mtRecebida) { //aqui seta a maquina inicia, q é ir pra esquerda para marcar o começo da fita
    Mt **mt = malloc(sizeof(Mt*)*4);
    int i=0;
    for(i=0;i<4;i++) {
        mt[i] = malloc(sizeof(Mt));
    }

    strcpy(mt[0]->currState, "0\0");
    strcpy(mt[0]->currSymbol, "_\0");
    strcpy(mt[0]->newSymbol, "¢\0");
    strcpy(mt[0]->direction, "l\0");
    strcpy(mt[0]->newState, novoEstado);

    strcpy(mt[1]->currState, "0\0");
    strcpy(mt[1]->currSymbol, "1\0");
    strcpy(mt[1]->newSymbol, "1\0");
    strcpy(mt[1]->direction, "l\0");
    strcpy(mt[1]->newState, novoEstado);

    strcpy(mt[2]->currState, "0\0");
    strcpy(mt[2]->currSymbol, "0\0");
    strcpy(mt[2]->newSymbol, "0\0");
    strcpy(mt[2]->direction, "l\0");
    strcpy(mt[2]->newState, novoEstado);

    strcpy(mt[3]->currState, novoEstado);
    strcpy(mt[3]->currSymbol, "_\0");
    strcpy(mt[3]->newSymbol, "£\0");
    strcpy(mt[3]->direction, "r\0");
    strcpy(mt[3]->newState, mtRecebida[0]->currState);
    tam[1] = 4;
    return mt;

}

MtPendente **devolvePendencia(Mt *maquinaRecebida) { //a pendecia é os estados que tem movimento pra esquerda, eu crio um novo movimento nesse estado apra verificar se nao estamos no começo da fita
    MtPendente *mt = malloc(sizeof(MtPendente));
    strcpy(mt->currSymbol, "£\0");
    strcpy(mt->newSymbol, "£\0");
    strcpy(mt->direction, "r\0");
    mt->ativo = 1;
    strcpy(mt->newState, maquinaRecebida->newState);
    strcpy(mt->currState, maquinaRecebida->newState);

    return mt;
}

int acharPendenteDuplicado(MtPendente **mtPendente, char *estado, int *tam) { //verificar se o estado pendente ja esta listado
    int i=0;

    for(i=0;i<tam[2]-1;i++) {
        if(mtPendente[i]->currState[0] == estado[0] && mtPendente[i]->currState[1] == estado[1]) {
            return 1;
        }
    }

return 0;
}

MtPendente *devolvePendente(Mt **maquinaRecebida, int *tam) { //aqui eu devolvo todos os estados pendentes
    MtPendente **mtPendente = malloc(sizeof(MtPendente));

    int i=0,k=0;

    for(i=0; i<tam[0]; i++) {
        if(maquinaRecebida[i]->direction[0] == 'l' ) {
            if(acharPendenteDuplicado(mtPendente, maquinaRecebida[i]->newState, tam) == 0) {
                mtPendente[k] = devolvePendencia(maquinaRecebida[i]);
                k++;
                tam[2]++;
                mtPendente = realloc(mtPendente, sizeof(MtPendente)*k+1);
            }
        }
    }
    return mtPendente;
}

int acharSymboloDuplicado(char **string, int tam, char *atual) { //verifica se esse simbolo ja n está no array de string
    int i=0;

    for(i=0;i<tam-1;i++) {
        if(string[i][0] == atual[0]) {
            return 1;
        }
    }
    return 0;
}

char **acharTodosOsSimbolos(Mt **maquinaRecebida, int *tam) { //aqui acha todos os simbolos q vao ser escritos durante o programa, para fazermos os * *
    int i=0, k;
    char **string = malloc(sizeof(char*)*2);
    for(i=0;i<2;i++){
        string[i] = malloc(sizeof(char)*2);
    }
    string[0][0] = '0';
    string[0][1] = '\0';
    string[1][0] = '1';
    string[1][1] = '\0';

    k=3;
    tam[3] = 3;
    for(i=0; i<tam[0]; i++) {

        if(acharSymboloDuplicado(string, k, maquinaRecebida[i]->currSymbol) == 0 && maquinaRecebida[i]->currSymbol[0]!= '*' && maquinaRecebida[i]->currSymbol[0] != '_') {
            string = realloc(string, sizeof(char*)*k);
            string[k-1] = malloc(sizeof(char)*2);
            string[k-1][0] = maquinaRecebida[i]->currSymbol[0];
            string[k-1][1] = '\0';
            k++;
            tam[3]++;
        }

        if(acharSymboloDuplicado(string, k, maquinaRecebida[i]->newSymbol) == 0 && maquinaRecebida[i]->newSymbol[0] != '_'
            && maquinaRecebida[i]->newSymbol[0] != '*') {
            string = realloc(string, sizeof(char*)*k);
            string[k-1] = malloc(sizeof(char)*2);
            string[k-1][0] = maquinaRecebida[i]->newSymbol[0];
            string[k-1][1] = '\0';
            k++;
            tam[3]++;
        }
    }
    return string;
}

int main()
{
    int *tam = malloc(sizeof(int)*4);
    int i=0;
    Mt **maquinaRecebida = lerMaquina(tam);

    if(maquinaRecebida != NULL) {

        char *s = acharNovoEstado(tam, maquinaRecebida);
        tam[3] = 1;
        tam[2]=1;
        MtPendente **mtPendente = devolvePendente(maquinaRecebida, tam);

        char **estados = acharTodosOsSimbolos(maquinaRecebida, tam);

        Mt **maquinaInicial = iniciaMaquinaInicial(tam,s , maquinaRecebida);
        tam[1] = 1;

        Mt **maquinaSaida = traduzirMt(maquinaRecebida, tam, maquinaInicial, estados, mtPendente);

        int k=0, p=0, m=0;
        k = tam[1]-1;
        Mt **mtUltima = malloc(sizeof(Mt*)*k);

        for(i=0; i<k; i++){
            mtUltima[i] = malloc(sizeof(Mt));
            maquinaSaida[i]->movidos=0;
        }

        for(i=0;i<k;i++) { //esse for vai ordenar os estados, pois devido a minha logica na tradução, tinha estados auxiliares no meio de alguns estados
            if(maquinaSaida[i]->movidos == 0){
                strcpy(mtUltima[m]->currState, maquinaSaida[i]->currState);
                strcpy(mtUltima[m]->currSymbol, maquinaSaida[i]->currSymbol);
                strcpy(mtUltima[m]->direction, maquinaSaida[i]->direction);
                strcpy(mtUltima[m]->newState, maquinaSaida[i]->newState);
                strcpy(mtUltima[m]->newSymbol, maquinaSaida[i]->newSymbol);
                m++;
            }

            if(i < k-1) {
                if(strcmp(mtUltima[i]->currState, maquinaSaida[i+1]->currState)!= 0) {
                    p=i+1;
                    while(p<k) {
                        if(strcmp(mtUltima[i]->currState, maquinaSaida[p]->currState) == 0){
                            if(maquinaSaida[p]->movidos == 0){
                                strcpy(mtUltima[m]->currState, maquinaSaida[p]->currState);
                                strcpy(mtUltima[m]->currSymbol, maquinaSaida[p]->currSymbol);
                                strcpy(mtUltima[m]->direction, maquinaSaida[p]->direction);
                                strcpy(mtUltima[m]->newState, maquinaSaida[p]->newState);
                                strcpy(mtUltima[m]->newSymbol, maquinaSaida[p]->newSymbol);
                                maquinaSaida[p]->movidos = 1;
                                m++;
                            }
                        }
                        p++;
                    }
                }

            }
        }

        FILE *fp = fopen("maquinaSaida.out", "w");

        for(i=0; i<k;i++) {
            fprintf(fp, "%s %s %s %s %s\n", mtUltima[i]->currState, mtUltima[i]->currSymbol,
                    mtUltima[i]->newSymbol, mtUltima[i]->direction, mtUltima[i]->newState);
        }

        printf("Maquina Traduzida");
    }
}
