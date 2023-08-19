#include "scoreboard.h"

void DrawScoreboard(SCORE *highscores)
{
    int i;
    char score [10];

    DrawText("HIGHSCORE", (WIDTH - MeasureText("HIGHSCORE",Mfonte+10))/2, 100, Mfonte+10, (Color){ 205, 109, 194, 255 });
    for(int i = 0; i < MAX_SCORES ; i++)
    {
        sprintf(score, "%d", highscores[i].score);
        DrawText(highscores[i].nome,  150, 250 + i * (Mfonte + E), Mfonte, PINK);
        DrawText(score,  (100-MeasureText(score,Mfonte))+ 950, 250 + i * (Mfonte + E), Mfonte, PINK);
    }
        //DrawText(scoreboard[i].score, (WIDTH/2 - MeasureText(scoreboard[i].score,Mfonte)/2), Yi + i * (Mfonte + E), Mfonte, LIGHTGRAY);

}

void le_arquivo_score(SCORE *scores[])
{
    FILE *arq;
    arq = fopen("highscores.bin", "rb");
    // lê arquivo e armazena no array
    fread(scores, sizeof(SCORE), MAX_SCORES, arq);
    fclose(arq);
}

void atualiza_highscores(SCORE hs[], SCORE nv)
{

    SCORE score_aux;
    for (int i = (MAX_SCORES - 1); i >= 0; i--){
        if (nv.score > hs[i].score){
                if (i == (MAX_SCORES - 1)){ // se i for (MAX_SCORES - 1) - ultimo lugar do placar - só armazena o novo score e descarta o score que estava na ultima posição
                    hs[i] = nv;
                }
                else { // senão
                    score_aux = hs[i]; // armazena o score atual da posição i numa auxiliar
                    hs[i] = nv; // atualiza o score da posição i com o novo score
                    hs[i+1] = score_aux; // o score antigo da posição i cai uma posição, sendo assim scores[i+]. score[3] vira score[4]
                }
        }
    }

}

void escreve_arquivo(SCORE* scores, char* nome_arq)
{
    FILE *p_arq;
    p_arq = fopen(nome_arq,"wb");
    if(p_arq!=NULL)
    {
        fwrite(scores, sizeof(SCORE), MAX_SCORES, p_arq);
        fclose(p_arq);
    }
}

/*
//atualiza apontuação
void atualiza_highscores(TIPO_SCORE hs[], TIPO_SCORE nv)
{
    int ind, continua, fim;
    fim = MAXSCORES-1;                     //sizeof(hs)??
    TIPO_SCORE aux;
    if( nv.score > hs[MAXSCORES-1].score)  //checa se a pontuação é maior que o ultimo da lista
    {
        hs[MAXSCORES-1]=nv;                //insere a pontuação mais abaixo da lista para fazer u buuble depois
    }
    for (ind = fim; ind > 0; ind--)
    {
        if( hs[ind].score > hs[ind-1].score )
        {
            aux = hs[ind-1];
            hs[ind-1] = hs[ind];
            hs[ind] = aux;
            //continua = 1;
        }
    }
    //}while(continua==1);

}

//entrar um novo jogador e pontuação
void le_score (TIPO_SCORE *novo)
{
    printf("Entre o nome do jogador:");
    gets(novo->nome);
    printf("Entre com sua pontuacao:");
    scanf(" %d", &novo->score);
}

//imprime o highscore
void imprime_hs (TIPO_SCORE *x)
{
    int i;
    for (i = 0; i < MAXSCORES; i++)
    {
        printf("%-20s%d\n", x[i].nome, x[i].score);
    }
}

//escreve_arquivo
void escreve_arquivo(TIPO_SCORE* scores, char* nome_arq)
{
    FILE *p_arq;
    p_arq = fopen(nome_arq,"wb");
    if(p_arq!=NULL)
    {
        fwrite(scores, sizeof(TIPO_SCORE), MAXSCORES, p_arq);
        fclose(p_arq);
    }
}


void le_arquivo_outro(TIPO_SCORE* scores, FILE* p_arq)
{
    fread(scores, sizeof(TIPO_SCORE), MAXSCORES, p_arq);
}


/*

int main (void)
{
    //inicializa variaveis
    TIPO_SCORE highscores[MAXSCORES];
    TIPO_SCORE novo;
    int i;
    char nome[16];
    FILE *p_arq;


    //solicitar nome de arquivo
    printf("Informe o nome de arquivo de entrada: ");
    gets(nome);//scanf("%s",&nome);
    //abra arquivo e chame le_arquivo
    if(!(p_arq = fopen(nome,"rb"))) //abre para acrescentar dados
        printf("Erro na abertura para acrescentar");
    else
    {
        le_arquivo_outro(highscores, p_arq);
        fclose(p_arq);
    }
    //*Imprime na tela
    printf("Lista inicial dos highscores:\n");
    imprime_hs(highscores);
    printf("\n");

    le_score(&novo);
    atualiza_highscores(highscores, novo);

    getchar();
    //solicitar arquivo escrita e chama escreve_arquivo
    printf("Informe o nome de arquivo de escrita: ");
    gets(nome);//scanf("%s",&nome);

    escreve_arquivo(highscores, nome);

    if(!(p_arq = fopen(nome,"rb")))
        printf("Erro na abertura para acrescentar");
    else
    {
        le_arquivo_outro(highscores, p_arq);
        fclose(p_arq);
    }
    printf("\n");
    printf("Lista final dos highscores:\n");
    imprime_hs(highscores);

    //le_arquivo e imprime_highscores


    //imprime_hs(novo);
    //printf("%-20s%d\n", novo.nome, novo.score);
    //printf("\n");
    //printf("Lista final dos highscores:\n");
    //imprime_hs(highscores);

    return 0;
}
*/
