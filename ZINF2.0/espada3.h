#include "raylib.h"
#include "Infos.h"
#define LINHAS 16
#define COLUNAS 24
#define TIMEATTACK 50
#define MAX_INIMIGO 10

typedef struct
{
    Rectangle rec;
    float velocidade;
    char rotacao;
    //Color cor;
    Texture2D direction;
}Sword;

void inicializa_espada(Sword *espada, Textures *sprites)
{
     espada->rec = (Rectangle)
    {
        x: 0,
        y: 0,
        width: sprites->attack_down.width,
        height: sprites->attack_down.height
    };
    espada->direction = sprites->attack_down;
    espada->rotacao = 'S';
    espada->velocidade = 400/FPS;

    return 0;
}

/*
void move_espada(Sword *espada)
{
    switch (espada->rotacao)
    {
    case 'W':
            espada->rec.y -= espada->velocidade;
        break;
    case 'S':
            espada->rec.y += espada->velocidade;
        break;
    case 'A':
            espada->rec.x -= espada->velocidade;
        break;
    case 'D':
            espada->rec.x += espada->velocidade;
        break;
    }
}
*/

void checa_colisao_espada(Character *enemy, char mapaJogo[][COLUNAS], Infos *settings, Character *player, Sword espada)
{

    int ja_morreu = 0;

    switch (player->rotacao)
    {
    case 'W':
        if(player->attackUp == true)
        {
            espada.rec.x = player->rec.x;
            espada.rec.y = player->rec.y-150;
            espada.rec.height = 150;
            espada.rec.width = 50;

            for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++)
            {
                if (ja_morreu == 0 && enemy[inimigo].vidas > 0)
                {
                    if (CheckCollisionRecs(espada.rec, enemy[inimigo].rec))
                    {
                        enemy[inimigo].vidas--;
                        //enemy[inimigo].rec.x = 0;
                        //enemy[inimigo].rec.y = 0;
                        ja_morreu = 1;
                        settings->score += 100;
                        settings->num_inimigos_mortos++;
                    }
                }
            }

            break;
        case 'S':
        if(player->attackDown == true)
        {
            espada.rec.x = player->rec.x;
            espada.rec.y = player->rec.y+50;
            espada.rec.height = 150;
            espada.rec.width = 50;
            for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++)
            {
                if (ja_morreu == 0 && enemy[inimigo].vidas > 0)
                {
                    if (CheckCollisionRecs(espada.rec, enemy[inimigo].rec))
                    {
                        enemy[inimigo].vidas--;
                        //enemy[inimigo].rec.x = 0;
                        //enemy[inimigo].rec.y = 0;
                        ja_morreu = 1;
                        settings->score += 100;
                        settings->num_inimigos_mortos++;
                    }
                }
            }
        }
            break;
        case 'A':
        if(player->attackLeft == true)
        {
            espada.rec.x = player->rec.x-150;
            espada.rec.y = player->rec.y;
            espada.rec.height = 50;
            espada.rec.width = 150;
            for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++)
            {
                if (ja_morreu == 0 && enemy[inimigo].vidas > 0)
                {
                    if (CheckCollisionRecs(espada.rec, enemy[inimigo].rec))
                    {
                        enemy[inimigo].vidas--;
                        //enemy[inimigo].rec.x = 0;
                        //enemy[inimigo].rec.y = 0;
                        ja_morreu = 1;
                        settings->score += 100;
                        settings->num_inimigos_mortos++;
                    }

                }
            }
        }
            break;
        case 'D':
        if(player->attackRight == true)
        {
            espada.rec.x = player->rec.x+50;
            espada.rec.y = player->rec.y;
            espada.rec.height = 50;
            espada.rec.width = 150;
            for (int inimigo = 0; inimigo < settings->num_inimigos; inimigo++)
            {
                if (ja_morreu == 0 && enemy[inimigo].vidas > 0)
                {
                    if (CheckCollisionRecs(espada.rec, enemy[inimigo].rec))
                    {
                        enemy[inimigo].vidas--;
                        //enemy[inimigo].rec.x = 0;
                        //enemy[inimigo].rec.y = 0;
                        ja_morreu = 1;
                        settings->score += 100;
                        settings->num_inimigos_mortos++;
                    }

                }
            }
        }
            break;
        }
    }
}

void checa_ataque(Character *player, Sword *espada, Textures *sprites)
{
    switch(player->rotacao)
    {
    case 'S':
      //  player->attackUp = false;
      //  player->attackLeft = false;
      //  player->attackRight = false;
        if (IsKeyPressed(KEY_J) && player->attackTimer == 0)
        {
            player->attackDown = true;
            espada->rec = player->rec;
            espada->rotacao = 'S';
            espada->direction = sprites->attack_down;
        }

        break;
    case 'W':
      //  player->attackDown = false;
      //  player->attackLeft = false;
     //   player->attackRight = false;
        if (IsKeyPressed(KEY_J) && player->attackTimer == 0)
        {
            player->attackUp = true;
            espada->rec = player->rec;
            espada->rotacao = 'W';
            espada->direction = sprites->attack_up;
        }

        break;
    case 'D':
       // player->attackUp = false;
      //  player->attackDown = false;
      //  player->attackLeft = false;
        if (IsKeyPressed(KEY_J) && player->attackTimer == 0)
        {
            player->attackRight = true;
            espada->rec = player->rec;
            espada->rotacao = 'D';
            espada->direction = sprites->attack_right;
        }

        break;
    case 'A':
       // player->attackUp = false;
       // player->attackDown = false;
       // player->attackRight = false;
        if (IsKeyPressed(KEY_J) && player->attackTimer == 0)
        {
            player->attackLeft = true;
            espada->rec = player->rec;
            espada->rotacao = 'A';
            espada->direction = sprites->attack_left;
        }

        break;
    }
                if(player->attackUp == true)
            {
                for(int s = 0 ; s<3 ; s++){
                DrawTexture(espada->direction, espada->rec.x, espada->rec.y-s*50-50, GREEN);
                }
                player->attackTimer += 10;
                if(player->attackTimer > TIMEATTACK)
                {
                     player->attackUp = false;
                     player->attackTimer = 0;
                }
            }
            if(player->attackDown == true)
            {
                for(int s = 0 ; s<3 ; s++){
                DrawTexture(espada->direction, espada->rec.x, espada->rec.y+s*50+50, GREEN);
                }
                player->attackTimer += 10;
                if(player->attackTimer > TIMEATTACK)
                {
                     player->attackDown = false;
                     player->attackTimer = 0;
                }
            }
                        if(player->attackLeft == true)
            {
                for(int s = 0 ; s<3 ; s++){
                DrawTexture(espada->direction, espada->rec.x-s*50-50, espada->rec.y , GREEN);
                }
                player->attackTimer += 10;
                if(player->attackTimer > TIMEATTACK)
                {
                     player->attackLeft = false;
                     player->attackTimer = 0;
                }
            }
                        if(player->attackRight == true)
            {
                for(int s = 0 ; s<3 ; s++){
                DrawTexture(espada->direction, espada->rec.x+s*50+50, espada->rec.y , GREEN);
                }
                player->attackTimer += 10;
                if(player->attackTimer > TIMEATTACK)
                {
                     player->attackRight = false;
                     player->attackTimer = 0;
                }
            }
}
