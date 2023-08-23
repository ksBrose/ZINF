#include <stdio.h>
#include <string.h>
#include "raylib.h"
#define HEIGHT 860
#define WIDTH 1200
#define Mfonte 60
#define E 20
#define MAX_SCORES 5
#define MAX_INPUT_CHARS 15

typedef struct tipo_score
{
 char nome[20];
 int score;
} SCORE;

void DrawScoreboard(SCORE *highscores);

void le_arquivo_score(SCORE *scores[]);

void atualiza_highscores(SCORE hs[], SCORE nv);

void escreve_arquivo(SCORE* scores, char* nome_arq);
