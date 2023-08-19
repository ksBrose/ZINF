#include "raylib.h"
#include "Infos.h"
#define LINHAS 16
#define COLUNAS 24
#define TIMEATTACK 250
#define MAX_INIMIGO 10

typedef struct
{
    Rectangle rec;
    float velocidade;
    char rotacao;
    //Color cor;
    Texture2D direction;
} Sword;

void inicializa_espada(Sword *espada, Textures *sprites)
{
    espada->rec = (Rectangle)
    {
        x: 0,
        y: 0,
width:
        sprites->attack_down.width,
height:
        sprites->attack_down.height
    };
    espada->direction = sprites->attack_down;
    espada->rotacao = 'S';
    espada->velocidade = 400/FPS;

    return 0;
}

void move_espada(Sword *espada, Character *player)
{
    switch (espada->rotacao)
    {
    case 'W':
                if(player->attackUp == true)
        {
        espada->rec.y -= espada->velocidade;
        }
        break;
    case 'S':
                if(player->attackDown == true)
        {
        espada->rec.y += espada->velocidade;
        }
        break;
    case 'A':
                if(player->attackLeft == true)
        {
        espada->rec.x -= espada->velocidade;
        }
        break;
    case 'D':
                if(player->attackRight == true)
        {
        espada->rec.x += espada->velocidade;
        }
        break;
    }
}

void checa_colisao_espada(Character *enemy, char mapaJogo[][COLUNAS], Infos *settings, Character *player, Sword espada)
{


    int ja_morreu = 0;

    switch (player->rotacao)
    {
    case 'W':
        if(player->attackUp == true)
        {

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
                        player->attackUp = false;
                        player->attackTimer = 0;
                    }
                }
            }

            break;
        case 'S':

            if(player->attackDown == true)
            {

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
                            player->attackDown = false;
                            player->attackTimer = 0;
                        }
                    }
                }
            }
            break;
        case 'A':

            if(player->attackLeft == true)
            {

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
                            player->attackLeft = false;
                            player->attackTimer = 0;
                        }

                    }
                }
            }
            break;
        case 'D':

            if(player->attackRight == true)
            {

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
                            player->attackRight = false;
                            player->attackTimer = 0;
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
        if (IsKeyPressed(KEY_J) && player->attackTimer == 0)
        {
            player->attackDown = true;
            espada->rec = player->rec;
            espada->rotacao = 'S';
            espada->direction = sprites->attack_down;
        }

        break;
    case 'W':
        if (IsKeyPressed(KEY_J) && player->attackTimer == 0)
        {
            player->attackUp = true;
            espada->rec = player->rec;
            espada->rotacao = 'W';
            espada->direction = sprites->attack_up;
        }

        break;
    case 'D':
        if (IsKeyPressed(KEY_J) && player->attackTimer == 0)
        {
            player->attackRight = true;
            espada->rec = player->rec;
            espada->rotacao = 'D';
            espada->direction = sprites->attack_right;
        }

        break;
    case 'A':
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
        DrawTexture(espada->direction, espada->rec.x, espada->rec.y, GREEN);
        player->attackTimer += 10;
        if(player->attackTimer > TIMEATTACK)
        {
            player->attackUp = false;
            player->attackTimer = 0;
            espada->rec.x=0;
            espada->rec.y=0;
        }
    }
    if(player->attackDown == true)
    {
        DrawTexture(espada->direction, espada->rec.x, espada->rec.y, GREEN);
        player->attackTimer += 10;
        if(player->attackTimer > TIMEATTACK)
        {
            player->attackDown = false;
            player->attackTimer = 0;
            espada->rec.x=0;
            espada->rec.y=0;
        }
    }
    if(player->attackLeft == true)
    {
        DrawTexture(espada->direction, espada->rec.x, espada->rec.y, GREEN);
        player->attackTimer += 10;
        if(player->attackTimer > TIMEATTACK)
        {
            player->attackLeft = false;
            player->attackTimer = 0;
            espada->rec.x=0;
            espada->rec.y=0;
        }
    }
    if(player->attackRight == true)
    {
        DrawTexture(espada->direction, espada->rec.x, espada->rec.y, GREEN);
        player->attackTimer += 10;
        if(player->attackTimer > TIMEATTACK)
        {
            player->attackRight = false;
            player->attackTimer = 0;
            espada->rec.x=0;
            espada->rec.y=0;
        }
    }
}
