#include "scoreboard.h"

/*Dado um array com os highscores, o desenha na tela.*/
void DrawScoreboard(SCORE *highscores)
{
    int i;
    char score [10];

    DrawText("HIGHSCORE", (WIDTH - MeasureText("HIGHSCORE",Mfonte+10))/2, 100, Mfonte+10, (Color){ 205, 109, 194, 255 });
    for(i = 0; i < MAX_SCORES ; i++)
    {
        sprintf(score, "%d", highscores[i].score);
        DrawText(highscores[i].nome,  150, 250 + i * (Mfonte + E), Mfonte, PINK);
        DrawText(score,  (100-MeasureText(score,Mfonte))+ 950, 250 + i * (Mfonte + E), Mfonte, PINK);
    }
}

/*Dado um array de scores, lê o arquivo ranking.bin e preenche o array fornecido.*/
void le_arquivo_score(SCORE *scores)
{
    FILE *arq;
    arq = fopen("ranking.bin", "rb");
    // lê arquivo e armazena no array
    fread(scores, sizeof(SCORE), MAX_SCORES, arq);
    fclose(arq);
}

/*Dado um array de highscores e um score, atualiza o array com o novo score caso o score fornecido for maior que algum dos termos de highscores.*/
void atualiza_highscores(SCORE hs[], SCORE nv)
{

    SCORE score_aux;
    for (int i = (MAX_SCORES - 1); i >= 0; i--){
        if (nv.score > hs[i].score){
                if (i == (MAX_SCORES - 1)){ // se i for (MAX_SCORES - 1) - ultimo lugar do placar - só armazena o novo score e descarta o score que estava na ultima posição
                    hs[i] = nv;
                }
                else { // se não
                    score_aux = hs[i]; // armazena o score atual da posição i numa auxiliar
                    hs[i] = nv; // atualiza o score da posição i com o novo score
                    hs[i+1] = score_aux; // o score antigo da posição i cai uma posição, sendo assim scores[i+]. score[3] vira score[4]
                }
        }
    }

}

/*Dado um array de scores e o nome de um arquivo, escreve o array de scores no arquivo com o nome fornecido.*/
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

/*Dado um contador, uma variavel para armazenar o nome e um contador de frames, preenche a o array de char com o nome digitado.*/
void prog_save(int *letterCount,char *name, int framesCounter)
{
    //input do usuario para salvar nome para salvar no highscore
    // Get char pressed (unicode character) on the queue
    int key = GetCharPressed();

    // Check if more characters have been pressed on the same frame
    while (key > 0)
    {
        // NOTE: Only allow keys in range [32..125] TABELA ASCII
        if ((key >= 32) && (key <= 125) && (*letterCount < MAX_INPUT_CHARS))
        {
            name[*letterCount] = (char)key;
            name[*letterCount+1] = '\0'; // Add null terminator at the end of the string.
            *letterCount += 1;
        }

        key = GetCharPressed();  // Check next character in the queue
    }
    //se Backspacec pressionado e o contador de letras digitadas for 1
    if (IsKeyPressed(KEY_BACKSPACE) && *letterCount>0)
    {
        *letterCount -= 1;  //volta uma caractere
        name[*letterCount] = '\0';  //e bota \0 no lugar
    }
    framesCounter++;  //variavel para piscar underline
}

