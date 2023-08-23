#include "raylib.h"
#include "math.h"
#define LINHAS 16
#define COLUNAS 24

void le_nivel(Textures *textures, Infos *settings, char mapaJogo[][COLUNAS], SCORE *player_score, Character *player)
{
    //le arquivo nivel X e salva na variavel mapaJogo
    FILE *arq_nivel;
    char caractere, arq_nome[30];

    settings->num_inimigos = 0; // zera a contagem pois o nivel é lido varias vezes durante a execução do jogo

    sprintf(arq_nome, "nivel%d.txt", settings->currentLevel); // retorna uma string nivel(x).txt na variavel arq_nivel onde x é o nivel atual

    arq_nivel = fopen(arq_nome, "r");


    if(arq_nivel != NULL) //checa se foi possivel abrir o nivel
    {
        for (int i = 0; i < LINHAS; i++)
        {
            for (int j = 0; j < COLUNAS + 1; j++) // fim dos caracteres mais "\n"
            {
                caractere = getc(arq_nivel);
                if (caractere != '\n' || caractere != '\0')
                {
                    mapaJogo[i][j] = caractere;
                }
                if (caractere == 'M')
                    settings->num_inimigos++;   //conta quantos inimigos tem no nivel
            }
        }
        fclose(arq_nivel);
    }
    else                //se não termina o jogo
    {
        if(settings->currentMode == ENDLESS)
        {
            settings->currentLevel = 1;
        }
        else
        {
            player_score->score = settings->score;
            settings->currentState = GAMEOVER;
        }
    }
}

void display_jogo(Character *player, Character enemy[], char mapaJogo[][COLUNAS], Textures *sprites, Infos *settings)
{
    char nivel[7], vidas[2], escore[10];
    //char inimigos_mortos[10], inimigos[10];
    Rectangle areia, pedras;

    ClearBackground(BLACK);
    DrawRectangleRec(settings->gameArea, BROWN);


//  Desenha barra de status

    sprintf(vidas, "VIDAS: %d", player->vidas);
    DrawText(vidas, 30, 15, 30, WHITE);

    sprintf(nivel, "NIVEL: %d", settings->currentLevel);
    DrawText(nivel, 185, 15, 30, WHITE);

    sprintf(escore, "ESCORE: %d", settings->score);
    DrawText(escore, 330, 15, 30, WHITE);


//  Desenha chao (em todos elementos do mapa)

    for(int linha = 0; linha < 16; linha++)
    {
        for(int coluna = 0; coluna < 24; coluna++)
        {
            areia.x = (coluna*50) + settings->gameArea.x;
            areia.y = (linha*50) + settings->gameArea.y;
            DrawTexture(sprites->ground, areia.x, areia.y, WHITE);
        }
    }

//  Desenha obstaculos (checa todos os elementos possiveis)

    for(int linha = 0; linha < 16; linha++)
    {
        for(int coluna = 0; coluna < 24; coluna++)
        {
            if(mapaJogo[linha][coluna] == 'P')
            {
                pedras.x = (coluna*50) + settings->gameArea.x;
                pedras.y = (linha*50) + settings->gameArea.y;
                DrawTexture(sprites->obstacle, pedras.x, pedras.y, WHITE);
            }
        }
    }

//  Desenha monstros com sprite com respectivo sentido de movimento

    for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++)
    {
        if (enemy[inimigo].vidas > 0)
        {
            switch(enemy[inimigo].rotacao)
            {
            case 'W':
                enemy[inimigo].direction = sprites->enemy_up;
                break;
            case 'S':
                enemy[inimigo].direction = sprites->enemy_down;
                break;
            case 'A':
                enemy[inimigo].direction = sprites->enemy_left;
                break;
            case 'D':
                enemy[inimigo].direction = sprites->enemy_right;
                break;
            }
            DrawTexture(enemy[inimigo].direction, enemy[inimigo].rec.x, enemy[inimigo].rec.y, WHITE);
        }
    }


//  Desenha jogador com sprite com respectivo sentido de movimento
    if (player->vidas > 0)
    {
        switch (player->rotacao)
        {
        case 'W':
            player->direction = sprites->player_up;

            break;
        case 'S':
            player->direction = sprites->player_down;

            break;
        case 'A':
            player->direction = sprites->player_left;

            break;
        case 'D':
            player->direction = sprites->player_right;

            break;
        }
        DrawTexture(player->direction, player->rec.x, player->rec.y, WHITE);
    }
}

void checa_colisao_mapa(Character *player, char mapaJogo[][COLUNAS], Infos *settings, Textures *sprites, Sword *espada)
{
    // Reseta os status de colisao do personagem
    player->collisionUp = false;
    player->collisionDown = false;
    player->collisionRight = false;
    player->collisionLeft = false;

    // Checa colisao com os limites do mapa
    // WIDTH - 50 = Largura max do mapa - 50 pixel (tamanho do sprite do corpo do link), se a pos atual for maior do que isso significa que ao dar o prox mov o corpo ja ficará pra fora do mapa
    if (player->rec.x >= WIDTH - 50)
    {
        player->collisionRight = true;
    }
    // se a pos x for menor que 0 ao dar o prox mov o corpo ficará pra fora do mapa
    if (player->rec.x <= 0)
    {
        player->collisionLeft = true;
    }
    // se a pos y for menor que 65 (o espaco que o link se move a cada aperto de tecla + 60 de deslocamento de altura sem a barra de status) significa que ao dar o prox mov o corpo ficará pra fora do mapa
    if (player->rec.y <= 60 + 0)
    {
        player->collisionUp = true;
    }
    // HEIGHT - 50 = Altura max do mapa - 50 pixel (tamanho do sprite do corpo do link), se a pos atual for maior do que isso significa que ao dar o prox mov o corpo ja ficará pra fora do mapa
    if (player->rec.y >= HEIGHT - 50)
    {
        player->collisionDown = true;
    }

// Checa colisao com as pedras (player e espada)
    Rectangle pedra;

    for (int linha = 0; linha < LINHAS; linha++)
    {
        for (int coluna = 0; coluna < COLUNAS; coluna++)
        {
            if (mapaJogo[linha][coluna] == 'P')
            {
                pedra.x = (coluna*50) + settings->gameArea.x;
                pedra.y = (linha*50) + settings->gameArea.y;
                pedra.width = sprites->obstacle.width - 15;
                pedra.height = sprites->obstacle.height - 15;

                if(CheckCollisionRecs(espada->rec, pedra))
                {
                    player->attackUp = false;
                    player->attackDown = false;
                    player->attackLeft = false;
                    player->attackRight = false;
                    espada->rec.x=0;
                    espada->rec.y=0;
                    player->attackTimer = 0;
                    PlaySound(espada->hitwall);
                }

                if (player->rotacao == 'W')
                {
                    while(CheckCollisionRecs(player->rec, pedra))
                    {
                        player->collisionUp = true;
                        player->rec.y++; // joga o player 1 pixel para baixo, de forma a nao colidir com o objeto na direcao W
                        player->rec.y = floor(player->rec.y); // seta o y do player em seu menor valor
                    }
                }
                else if (player->rotacao == 'S')
                {
                    while(CheckCollisionRecs(player->rec, pedra))
                    {
                        player->collisionDown = true;
                        player->rec.y--;
                        player->rec.y = ceil(player->rec.y);
                    }
                }
                else if (player->rotacao == 'D')
                {
                    while(CheckCollisionRecs(player->rec, pedra))
                    {
                        player->collisionRight = true;
                        player->rec.x--;
                        player->rec.x = ceil(player->rec.x);
                    }
                }
                else if (player->rotacao == 'A')
                {
                    while(CheckCollisionRecs(player->rec, pedra))
                    {
                        player->collisionLeft = true;
                        player->rec.x++;
                        player->rec.x = floor(player->rec.x);
                    }
                }
            }
        }
    }
}

